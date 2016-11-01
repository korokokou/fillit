#include "fillit.h"

static inline t_bool		set(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	int						new_offset;

	new_offset = t->offset.y + t->offset.x;
	value = (t->new_value >> new_offset);
	if (value & map->grid[0])
		return (FALSE);
	map->grid[0] |= value;
	t->new_offset[0] = new_offset;	
	return (TRUE);
}

static inline void			unset(t_map *map, t_tetrimino *t)
{
	uint64_t				value;
	int						new_offset;

	new_offset = t->offset.y + t->offset.x;
	value = (t->new_value >> new_offset);
	map->grid[0] ^= value;
}

int							check_dead_zones(t_map *map)
{
	int						temp;
	int						temp2;

	print_dyn_piece(map->grid[0], map->size);
	print_dyn_piece(map->mask, map->size);
	temp = ~(map->grid[0] & map->mask);
	print_dyn_piece(temp, map->size);
	temp2 = ~map->grid[0];
	print_dyn_piece(temp2, map->size);
	temp2 = temp2 & temp;
	print_dyn_piece(temp2, map->size);
	ft_putendl("*******************************");
	return 1;
}

t_bool						resolve(t_map *map, int tetri_index, int const size)
{
	t_tetrimino				*t;
	t_pos					dyn_pos_backup;

	t = &map->t[tetri_index];
	t->offset = map->dyn_pos[t->pattern_index];
	dyn_pos_backup = map->dyn_pos[t->pattern_index];
	t->offset.x += (t->offset.x > 0) ? t->gap_x : 0;
	while (t->offset.y <= t->max_offset)
	{
		while (t->offset.x <= t-> limit_line)
		{
			if (set(map, t))
			{
				map->dyn_pos[t->pattern_index] = t->offset;
				if (((tetri_index + 1 >= map->t_count)
					|| (/*check_dead_zones(map)
						&&*/ resolve(map, tetri_index + 1, size)
						)))
					return (1);
				unset(map, t);
			}
			t->offset.x++;
		}
		t->offset.y += size;
		t->offset.x = 0;
	}
	map->dyn_pos[t->pattern_index] = dyn_pos_backup;
	return (0);
}
