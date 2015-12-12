#include <gof.h>
#include <bandit/bandit.h>
#include <algorithm>

using namespace bandit;
using namespace gof;

go_bandit([]{
  describe("Cells state", []{
    it("initial state", []{
      AssertThat(cell(false).is_alive(), IsFalse());
      AssertThat(cell(true).is_alive(), IsTrue());
    });

    it("dead stays dead", []{
      AssertThat(cell(false).mutate(0).is_alive(), IsFalse());
      AssertThat(cell(false).mutate(1).is_alive(), IsFalse());
      AssertThat(cell(false).mutate(4).is_alive(), IsFalse());
      AssertThat(cell(false).mutate(5).is_alive(), IsFalse());
      AssertThat(cell(false).mutate(6).is_alive(), IsFalse());
      AssertThat(cell(false).mutate(7).is_alive(), IsFalse());
      AssertThat(cell(false).mutate(8).is_alive(), IsFalse());
    });

    it("dead with tree eighbours lives", []{
      AssertThat(cell(false).mutate(3).is_alive(), IsTrue());
    });

    it("live cell dies", []{
      AssertThat(cell(true).mutate(0).is_alive(), IsFalse());
      AssertThat(cell(true).mutate(1).is_alive(), IsFalse());
      AssertThat(cell(true).mutate(4).is_alive(), IsFalse());
      AssertThat(cell(true).mutate(5).is_alive(), IsFalse());
      AssertThat(cell(true).mutate(6).is_alive(), IsFalse());
      AssertThat(cell(true).mutate(7).is_alive(), IsFalse());
    });

    it("live stays alive", []{
      AssertThat(cell(true).mutate(2).is_alive(), IsTrue());
      AssertThat(cell(true).mutate(3).is_alive(), IsTrue());
    });
  });

  describe("Coords", []{
    it("North", []{
      AssertThat(coord(0, 0).north(), Is().EqualTo(coord(0, -1)));
      AssertThat(coord(0, -1).north(), Is().EqualTo(coord(0, -2)));
      AssertThat(coord(0, -1).north(), Is().Not().EqualTo(coord(42, 89)));
      AssertThat(coord(1, 0).north(), Is().EqualTo(coord(1, -1)));
    });

    it("South", []{
      AssertThat(coord(0, 0).south(), Is().EqualTo(coord(0, 1)));
      AssertThat(coord(0, -1).south(), Is().EqualTo(coord(0, 0)));
      AssertThat(coord(0, -1).south(), Is().Not().EqualTo(coord(42, 89)));
      AssertThat(coord(1, 0).south(), Is().EqualTo(coord(1, 1)));
    });

    it("West", []{
      AssertThat(coord(0, 0).west(), Is().EqualTo(coord(-1, 0)));
      AssertThat(coord(1, 0).west(), Is().EqualTo(coord(0, 0)));
      AssertThat(coord(0, -1).west(), Is().Not().EqualTo(coord(42, 89)));
      AssertThat(coord(1, 1).west(), Is().EqualTo(coord(0, 1)));
    });

    it("East", []{
      AssertThat(coord(0, 0).east(), Is().EqualTo(coord(1, 0)));
      AssertThat(coord(-1, 0).east(), Is().EqualTo(coord(0, 0)));
      AssertThat(coord(0, -1).east(), Is().Not().EqualTo(coord(42, 89)));
      AssertThat(coord(-1, 1).east(), Is().EqualTo(coord(0, 1)));
    });

    it("South East", []{
      AssertThat(coord(0, 0).south_east(), Is().EqualTo(coord(1, 1)));
    });

    it("South West", []{
      AssertThat(coord(0, 0).south_west(), Is().EqualTo(coord(-1, 1)));
    });

    it("North West", []{
      AssertThat(coord(0, 0).north_west(), Is().EqualTo(coord(-1, -1)));
    });

    it("North East", []{
      AssertThat(coord(0, 0).north_east(), Is().EqualTo(coord(1, -1)));
    });

    it("Neighbours", []{
      auto n = coord(0, 0).neighbours();

      auto is_member = [&n](const coord &c) {
        return std::find(begin(n), end(n), c) != end(n);
      };

      AssertThat(is_member(coord(-1, -1)), IsTrue());
      AssertThat(is_member(coord(0, -1)), IsTrue());
      AssertThat(is_member(coord(1, -1)), IsTrue());
      AssertThat(is_member(coord(1, 0)), IsTrue());
      AssertThat(is_member(coord(1, 1)), IsTrue());
      AssertThat(is_member(coord(0, 1)), IsTrue());
      AssertThat(is_member(coord(-1, 1)), IsTrue());
      AssertThat(is_member(coord(-1, 0)), IsTrue());
      AssertThat(is_member(coord(42, 88)), IsFalse());
    });
  });

  describe("World", []{
    it("Add cell to World", []{
      AssertThat(world().live_length(), Is().EqualTo(0U));

      world one_cell_world = world().add_cell(coord(0, 0));

      AssertThat(one_cell_world.live_length(), Is().EqualTo(1U));
      AssertThat(one_cell_world.has_live_cell(coord(0, 0)), IsTrue());
      AssertThat(one_cell_world.live_neighbours_length(), Is().EqualTo(8U));
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(-1, -1)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(0, -1)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(1, -1)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(1, 0)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(1, 1)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(0, 1)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(-1, 0)), IsTrue());
      AssertThat(one_cell_world.has_dead_neighbour_cell(coord(0, 0)), IsFalse());
    });

    it("Add two distant cells", []{
      AssertThat(world().add_cell(coord(3, 6)).add_cell(coord(7, 10)).live_length(), Is().EqualTo(2U));
      AssertThat(world().add_cell(coord(3, 6)).add_cell(coord(7, 10)).live_neighbours_length(), Is().EqualTo(16U));
    });

    it("Add two close cells", []{
      AssertThat(world().add_cell(coord(3, 6)).add_cell(coord(4, 7)).live_length(), Is().EqualTo(2U));
      AssertThat(world().add_cell(coord(3, 6)).add_cell(coord(4, 7)).live_neighbours_length(), Is().EqualTo(12U));
    });

    it("Cell live neighbours", []{
      AssertThat(world()
                 .add_cell(coord(3, 6))
                 .add_cell(coord(4, 7))
                 .add_cell(coord(5, 6))
                 .count_live_neighbours(coord(4, 7)), Is().EqualTo(2U));

      AssertThat(world()
                 .add_cell(coord(3, 6))
                 .add_cell(coord(4, 7))
                 .add_cell(coord(100, 6))
                 .count_live_neighbours(coord(4, 7)), Is().EqualTo(1U));

      AssertThat(world()
                 .add_cell(coord(3, 34))
                 .add_cell(coord(4, 7))
                 .add_cell(coord(100, 6))
                 .count_live_neighbours(coord(4, 7)), Is().EqualTo(0U));

      AssertThat(world()
                 .count_live_neighbours(coord(4, 7)), Is().EqualTo(0U));
    });
  });

  describe("Evolve", [] {
    it("Single cell results in empty world", []{
      AssertThat(world()
                 .add_cell(coord(0, 0))
                 .evolve()
                 .live_length(), Is().EqualTo(0U));
    });

    it("2x2 Square keeps constant", []{
      world evolved = world()
                       .add_cell(coord(1, 3))
                       .add_cell(coord(2, 3))
                       .add_cell(coord(1, 4))
                       .add_cell(coord(2, 4))
                       .evolve();

      AssertThat(evolved.live_length(), Is().EqualTo(4U));
      AssertThat(evolved.has_live_cell(coord(1, 3)), IsTrue());
      AssertThat(evolved.has_live_cell(coord(2, 3)), IsTrue());
      AssertThat(evolved.has_live_cell(coord(1, 4)), IsTrue());
      AssertThat(evolved.has_live_cell(coord(2, 4)), IsTrue());
      AssertThat(evolved.has_live_cell(coord(2, 4)), IsTrue());
      AssertThat(evolved.has_live_cell(coord(23, 42)), IsFalse());
    });

    it("tree cells inline rotates", []{
      world evolved = world()
                       .add_cell(coord(4, 1))
                       .add_cell(coord(4, 2))
                       .add_cell(coord(4, 3))
                       .evolve();

      AssertThat(evolved.live_length(), Is().EqualTo(3U));

      // it's cyclic of period 1
      world original = evolved.evolve();

      AssertThat(original.live_length(), Is().EqualTo(3U));
      AssertThat(original.has_live_cell(coord(4, 1)), IsTrue());
      AssertThat(original.has_live_cell(coord(4, 2)), IsTrue());
      AssertThat(original.has_live_cell(coord(4, 2)), IsTrue());
      AssertThat(original.has_live_cell(coord(45, 2)), IsFalse());
    });

  });
});

int main(int argc, char **argv)
{
  return run(argc, argv);
}
