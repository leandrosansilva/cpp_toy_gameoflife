#pragma once

#include <array>
#include <map>
#include <algorithm>
#include <tuple>

namespace gof {

struct cell
{
  bool _state;

  explicit cell(bool state):
  _state(state)
  {
  }

  bool is_alive() const
  {
    return _state;
  }

  cell mutate_dead(std::size_t neighbours_count) const
  {
    return cell{neighbours_count == 3};
  }

  cell mutate_alive(std::size_t neighbours_count) const
  {
    return cell{neighbours_count == 2 || neighbours_count == 3};
  }

  cell mutate(std::size_t neighbours_count) const
  {
    // NOTE: yes, this could be simpler. I just do not want to :-)
    return (this->*(_state ? &cell::mutate_alive : &cell::mutate_dead))(neighbours_count); 
  }
};

struct coord
{
  std::tuple<int, int> _v;

  explicit coord(int x, int y):
  _v(std::make_tuple(x, y))
  {
  }

  int x() const
  {
    return std::get<0>(_v);
  }

  int y() const
  {
    return std::get<1>(_v);
  }


  bool operator==(const coord &other) const
  {
    return _v == other._v;
  }

  coord north() const
  {
    return coord{x(), y() - 1};
  }

  coord south() const
  {
    return coord{x(), y() + 1};
  }

  coord west() const
  {
    return coord{x() - 1, y()};
  }

  coord east() const
  {
    return coord{x() + 1, y()};
  }

  coord south_east() const
  {
    return south().east();
  }

  coord south_west() const
  {
    return south().west();
  }

  coord north_west() const
  {
    return north().west();
  }

  coord north_east() const
  {
    return north().east();
  }

  std::array<coord, 8> neighbours() const
  {
    return {
      north_west(),
      north(),
      north_east(),
      east(),
      south_east(),
      south(),
      south_west(),
      west()
    };
  }
};

struct coord_less
{
  bool operator()(const coord &c1, const coord &c2) const
  {
    return std::less<std::tuple<int, int>>()(c1._v, c2._v);
  }
};

struct world
{
  using coord_map = std::map<coord, bool, coord_less>;

  coord_map _coords;

  template<typename BinaryFunction>
  std::size_t accumulate_from_function(BinaryFunction f) const
  {
    return std::accumulate(std::begin(_coords), std::end(_coords), 0U, f);
  }

  std::size_t live_length() const
  {
    return accumulate_from_function([](std::size_t acc, const coord_map::value_type &pair) {
      return acc + (pair.second ? 1 : 0);
    });
  }

  world &add_cell(const coord &c)
  {
    _coords[c] = true;

    for (const coord &n: c.neighbours()) {
      if (!has_live_cell(n)) {
        _coords[n] = false;
      }
    }

    return *this;
  }

  bool has_live_cell(const coord &c) const
  {
    auto found = _coords.find(c);
    return found != std::end(_coords) && found->second;
  }

  std::size_t live_neighbours_length() const
  {
    return accumulate_from_function([](std::size_t acc, const coord_map::value_type &pair) {
      return acc + (pair.second ? 0 : 1);
    });
  }

  bool has_dead_neighbour_cell(const coord &c) const
  {
    auto found = _coords.find(c);
    return found != std::end(_coords) && !found->second;
  }

  std::size_t count_live_neighbours(const coord &c) const
  {
    auto n = c.neighbours();

    return std::accumulate(std::begin(n), std::end(n), 0U, [this](std::size_t acc, const coord &c) {
      auto found = _coords.find(c);
      return acc + (found == std::end(_coords) || !found->second ? 0 : 1);
    });
  }

  world evolve() const
  {
    world evolved;

    for (const coord_map::value_type &pair: _coords) {
      if (cell(pair.second).mutate(count_live_neighbours(pair.first)).is_alive()) {
        evolved.add_cell(pair.first);
      }
    }

    return evolved;
  }
};

}
