#include <unistd.h>
#include "fillit.h"
#include "libft.h"

#include <stdio.h> //

t_bool	is_correct_pattern(int value)
{
	static const int	correct_patterns[TETRI_PATTERNS_COUNT] = {
		57856, 17600, 36352, 51328, 58368, 19520, 19968, 35968,
		59392, 50240, 11776, 35008, 27648, 35904, 61440, 34952,
		52224, 50688, 19584
	};
	int					i;

	i = 0;
	while (i < TETRI_PATTERNS_COUNT)
	{
		if (value == correct_patterns[i])
			return (TRUE);
		i++;
	}
	return (FALSE);
}

int		move_to_most_top_left_position(int value)
{
	// move to most left position
	while ((value & LEFT_MASK) == 0)
		value = move(value, LEFT);
	// move to most top position
	while ((value & TOP_MASK) == 0)
		value = move(value, TOP);
	printf("value = %d\n");
	return (value);
}

int		raw_to_binary_represention(char *raw)
{
	int		i;
	int		value;

	value = 0x0000;
	i = 0xF;
	while (*raw)
	{
		// try [OPTI -> cast (int) move 4 by 4]
		if (*raw == CHAR_BLOCK)
			value |= 1 << i;
		raw++;
		i--;
	}
	// move tetrimino to most top-left position !!
	value = move_to_most_top_left_position(value);
	// check if valid
	if (!is_correct_pattern(value))
		error_msg_exit("invalid pattern");
	return (value);
}

int			parse(int fd, t_tetrimino tetriminos[MAX_TETRIMINOS])
{
	char	buffer[BUFFER_SIZE + 1];
	int		char_count;

	ft_bzero(buffer, BUFFER_SIZE + 1);
	// read
	char_count = read(fd, buffer, BUFFER_SIZE);
	if (char_count == -1)
		error_msg_exit("read failed");
	printf("read -> OK\n");

	// check length
	if (char_count == 0 || ((char_count + 1) % (MAX_TETRIMINO_SIZE + 1)) != 0)
		error_msg_exit("invalid file length");
	printf("check length -> OK\n");

	// check chars
	int sep;
	sep = 21;
	while (sep < char_count)
	{
		if (buffer[sep-1] != '\n')
			error_msg_exit("invalid separator: must be \\n");
		buffer[sep-1] = '@';
		sep += 21;
	}
	char	**tetriminos_tab;
	if (!(tetriminos_tab = ft_strsplit(buffer, '@')))
		error_msg_exit("split failed");

	char	**tmp;
	tmp = tetriminos_tab;
	int		tetri_index = 0;
	while (*tmp)
	{
		char	*tetrimino_raw_string;

		tetrimino_raw_string = *tmp;
		// parse tetri
		int		i;

		i = 0;
		while (i < MAX_TETRIMINO_SIZE)
		{
			if ((((i + 1) % 5) == 0 && tetrimino_raw_string[i] != '\n')
				&& (tetrimino_raw_string[i] != CHAR_BLOCK && tetrimino_raw_string[i] != CHAR_EMPTY))
				error_msg_exit("invalid char");
			i++;
		}
		char	**t_tab;

		if (!(t_tab = ft_strsplit(tetrimino_raw_string, '\n')))
			error_msg_exit("split 2 failed");

		char	**tmp2;
		char	*t_raw;

		t_raw = ft_strnew(16); //check if null
		tmp2 = t_tab;
		int j = 0;
		while (*tmp2)
		{
			ft_strcpy(t_raw + (j * 4), *tmp2);
			j++;
			tmp2++;
		}
		// setup tetrimino
		tetriminos[tetri_index].raw = t_raw;
		tetriminos[tetri_index].value = raw_to_binary_represention(t_raw);
		tetriminos[tetri_index].offset_x = 0;
		tetriminos[tetri_index].offset_y = 0;

		tmp++;
		tetri_index++;
	}
	free_tab(&tetriminos_tab);
	printf("check chars -> OK\n");
	return (tetri_index);
}