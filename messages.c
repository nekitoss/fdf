/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   messages.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:23:24 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:23:24 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "h_fdf.h"

void		error(char *str)
{
	perror(str);
	exit(1);
}

void		error_msg(char *str)
{
	ft_putendl(str);
	exit(1);
}

void		print_help(t_fdf *ls)
{
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 0, 0x1000FFFF,
		"W, A, S, D, Q, E : rotating in 3 axis");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 20, 0x1000FFFF,
		"arrows : moving around");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 40, 0x1000FFFF,
		"- , +, 0 : for zoom");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 60, 0x1000FFFF,
		"C : disable colors");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 80, 0x1000FFFF,
		"R : reset angles to zero");
	mlx_string_put(ls->mlx_ptr, ls->win_ptr, 0, 100, 0x1000FFFF,
		"L : enable / disable lines");
}
