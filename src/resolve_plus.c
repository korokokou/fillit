#include "fillit.h"

static inline t_bool		set_plus(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	uint64_t				value2;
	int						grid_plus_offset;
	int						new_offset;
	int						grid;

	new_offset = t->offset.y + t->offset.x;
	grid = new_offset > t->max_grid[1];
	value = t->new_value;
	value >>= new_offset;
	if (value & map->grid[grid] && new_offset < (64 << grid))
		return (FALSE);
	else if (new_offset > t->max_grid[grid])
	{
		grid_plus_offset = new_offset - (63 << grid);
		if (grid_plus_offset > 0)
			value2 = (t->new_value >> grid_plus_offset);
		else
			value2 = (t->new_value << -grid_plus_offset);
		if (value2 & map->grid[grid + 1])
			return (FALSE);
		map->grid[grid + 1] |= value2;
	}
	map->grid[grid] |= value;
	t->new_offset = new_offset;
	return (TRUE);
}

static inline void			unset_plus(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	uint64_t				value2;
	int						grid_plus_offset;
	int						grid;


	grid = t->new_offset > t->max_grid[1];
	value = t->new_value;
	value >>= t->new_offset;
	if (t->new_offset < 64)
		map->grid[grid] ^= value;
	if (t->new_offset > t->max_grid[grid])
	{
		grid_plus_offset = t->new_offset - (63);
		if (grid_plus_offset > 0)
			value2 = (t->new_value >> grid_plus_offset);
		else
			value2 = (t->new_value << -grid_plus_offset);
		map->grid[grid + 1] ^= value2;
	}
}

t_bool		resolve_plus(t_map *map, int tetri_index, int const size)
{
	t_tetrimino		*t;
	t_pos			dyn_pos_backup;

	t = &map->t[tetri_index];
	t->offset = map->dyn_pos[t->pattern_index];
	dyn_pos_backup = map->dyn_pos[t->pattern_index];
	t->offset.x += (t->offset.x > 0) ? t->gap_x : 0;
	while (t->offset.y <= t->max_offset)
	{
		while (t->offset.x <= t->limit_line)
		{
			if (set_plus(map, t))
			{
				map->dyn_pos[t->pattern_index] = t->offset;
				if (((tetri_index + 1 >= map->t_count)
							|| resolve_plus(map, tetri_index + 1, size)))
					return (1);
				unset_plus(map, t);
			}
			t->offset.x++;
		}
		t->offset.y += size;
		t->offset.x = 0;
	}
	map->dyn_pos[t->pattern_index] = dyn_pos_backup;
	return (0);
}
