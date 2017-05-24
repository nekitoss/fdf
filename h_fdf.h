/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   h_fdf.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nekitoss <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/24 21:22:18 by nekitoss          #+#    #+#             */
/*   Updated: 2017/05/24 21:22:19 by nekitoss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef H_FDF_H
# define H_FDF_H
# include <math.h>
# include "libft/libft.h"
# include "mlx.h"
# define FLOAT_T float
# define IMG_ROW(a) (a * (ls->size_line))
# define IMG_COL(b) (b * (ls->bits_per_pixel / 8))
# define WHITE 0x00FFFFFF

typedef struct	s_point
{
	FLOAT_T		x;
	FLOAT_T		y;
	FLOAT_T		z;
	int			color;
}				t_point;

typedef struct	s_fdf
{
	t_point		**arr;
	t_point		**orig;
	char		**text;
	char		*line;
	char		*addr;
	char		*data;
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img_ptr;
	int			fd;
	int			ret;
	int			num_rows;
	int			num_cols;
	int			size_x;
	int			size_y;
	int			x1;
	int			y1;
	int			x2;
	int			y2;
	int			bits_per_pixel;
	int			endian;
	int			size_line;
	int			angle_x;
	int			angle_y;
	int			angle_z;
	int			center_x;
	int			center_y;
	int			max_h;
	int			min_h;
	int			shift_x;
	int			shift_y;
	int			d;
	int			d1;
	int			d2;
	int			x;
	int			y;
	int			i;
	FLOAT_T		zoom;
	char		color_off;
	char		lines_off;
	char		help_on;
}				t_fdf;

typedef struct	s_img
{
	char		b;
	char		g;
	char		r;
	char		t;
}				t_img;

void			error(char *str);
void			error_msg(char *str);
void			print_help(t_fdf *ls);
void			del_struct(t_fdf *ls);
void			draw_line(t_fdf *ls);
void			print_lines(t_fdf *ls);
void			convert_row(t_fdf *ls, char **row, int r_num);
void			rows_into_array(t_fdf *ls);
void			count_save_rows(t_fdf *ls);
void			print_in_window(t_fdf *ls);
void			zoom_reset(t_fdf *ls);
void			zoom_in(t_fdf *ls);
void			zoom(t_fdf *ls, int step);
int				key_f(int key, t_fdf *ls);
void			make_image(t_fdf *ls);
void			make_lines(t_fdf *ls);
void			pixel_to_img(t_fdf *ls, int i, int j, int color);
void			redraw(t_fdf *ls);
void			rotate(t_fdf *ls);
void			make_color(t_fdf *ls);
void			find_height(t_fdf *ls);
void			key1(int key, t_fdf *ls);

#endif
