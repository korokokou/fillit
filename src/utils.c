/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycribier <ycribier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/11 10:42:11 by ycribier          #+#    #+#             */
/*   Updated: 2015/12/11 10:48:01 by ycribier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		ft_ceil_sqrt(int n)
{
	int		i;

	i = 0;
	while (i * i < n)
		i++;
	return (i);
}
