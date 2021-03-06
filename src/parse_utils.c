#include "libft.h"
#include "fillit.h"

static void		check_tetri_chars(char *tetri_raw)
{
	int			i;
	char		c;

	i = 0;
	while (i < MAX_TETRIMINO_SIZE)
	{
		c = tetri_raw[i];
		if (((i + 1) % 5 == 0 && c != '\n')
			|| ((i + 1) % 5 != 0 && c != CHAR_BLOCK && c != CHAR_EMPTY))
			fillit_error_msg_exit("invalid char");
		i++;
	}
}

static void		parse_tetri(char *tetri_raw, t_tetrimino *t)
{
	char		**t_tab;
	char		*t_raw;
	int			i;

	if (!(t_tab = ft_strsplit(tetri_raw, '\n')))
		return (fillit_error_msg_exit("some split failed somewhere"));
	if (!(t_raw = ft_strnew(16)))
		return (fillit_error_msg_exit("some malloc failed somewhere"));
	i = 0;
	while (t_tab[i])
	{
		ft_strcpy(t_raw + (i * 4), t_tab[i]);
		i++;
	}
	free_tab(&t_tab);
	t->value = raw_to_binary_represention(t_raw);
	t->pattern_index = get_matched_pattern_index(t);
	if (t->pattern_index == -1)
		return (fillit_error_msg_exit("invalid pattern"));
	t->offset.x = 0;
	t->offset.y = 0;
	ft_strdel(&t_raw);
}

void			parse_entry(char *entry, t_map *map)
{
	char		**tetriminos_tab;
	char		**tmp_tab;
	int			tetri_index;

	if (!(tetriminos_tab = ft_strsplit(entry, '@')))
		return (fillit_error_msg_exit("some split failed somewhere"));
	tmp_tab = tetriminos_tab;
	tetri_index = 0;
	while (*tmp_tab)
	{
		check_tetri_chars(*tmp_tab);
		parse_tetri(*tmp_tab, &map->t[tetri_index]);
		tmp_tab++;
		tetri_index++;
	}
	free_tab(&tetriminos_tab);
	map->t_count = tetri_index;
}
