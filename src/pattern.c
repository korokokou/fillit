#include "fillit.h"

static const t_pattern		g_patterns[TETRI_PATTERNS_COUNT] = {
	{ 57856, 3, 3, 2 },
	{ 17600, 2, 2, 3 },
	{ 36352, 3, 3, 2 },
	{ 51328, 2, 2, 3 },
	{ 59392, 3, 3, 2 },
	{ 50240, 2, 2, 3 },
	{ 11776, 3, 3, 2 },
	{ 35008, 2, 2, 3 },
	{ 58368, 3, 3, 2 },
	{ 19520, 2, 2, 3 },
	{ 19968, 3, 3, 2 },
	{ 35968, 2, 2, 3 },
	{ 27648, 2, 3, 2 },
	{ 35904, 2, 2, 3 },
	{ 50688, 2, 3, 2 },
	{ 19584, 2, 2, 3 },
	{ 61440, 4, 4, 1 },
	{ 34952, 1, 1, 4 },
	{ 52224, 2, 2, 2 }
};

void		clear(t_map *map)
{
	int		i;
	int		index;

	i = 0;
	ft_bzero(map->dyn_pos, sizeof(map->dyn_pos));
	ft_bzero(map->grid, sizeof(int) * 3);
	map->resolve = (map->size < 9) ? resolve : resolve_plus;
//	map->mask = generate_dz_mask(map->size);
//	print_dyn_piece(map->mask, map->size);
	while (i < map->t_count)
	{
		index = map->t[i].pattern_index;
		map->t[i].offset.x = 0;
		map->t[i].offset.y = 0;
		map->t[i].new_value = move_to_most_top_left64_position(map->t[i].value);
		map->t[i].new_value = new_form(map->t[i].new_value, map->size);
		map->t[i].max_offset = map->total_space - ( g_patterns[index].height * map->size);
		map->t[i].max_grid = 64 - (g_patterns[index].height * map->size);
		map->t[i].limit_line = map->size - g_patterns[index].width;
	//	printf("%d\n", g_patterns[index].height);
		i++;
	}
}


int			get_matched_pattern_index(t_tetrimino *t)
{
	int		i;
	int		value;

	i = 0;
	value = t->value;
	while (i < TETRI_PATTERNS_COUNT)
	{
		if (value == g_patterns[i].value)
		{
		//	t->height = g_patterns[i].height;
		//	t->width = g_patterns[i].width;
			t->gap_x = g_patterns[i].gap_x;
			t->pattern_index = i;
			return (i);
		}
		i++;
	}
	return (-1);
}
