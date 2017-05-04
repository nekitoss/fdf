#ifndef H_FDF_H
# define H_FDF_H
// # include "assert.h"

# include "libft/libft.h"
// # include <mlx.h>
#include "minilibx_macos/mlx.h"

# define FLOAT_T float
# define IMG_ROW(a) (a * (ls->size_line))
# define IMG_COL(b) (b * (ls->bits_per_pixel / 8))

typedef struct	s_fdf
{
	int		fd;
	FLOAT_T	**arr;
	char	**text;
	char	*line;
	int		ret;
	int		num_rows;
	int		num_cols;
	char	*addr;
	void	*mlx_ptr;
	int		size_x;
	int		size_y;
	void	*win_ptr;
	int		zoom;
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	void	*img_ptr;
	int		bits_per_pixel;
	int		endian;
	int		size_line;
	char	*data;
}				t_fdf;

typedef struct	s_img
{
	char	b;
	char	g;
	char	r;
	char	t;
}				t_img;

void		printBits(size_t const size, void const * const ptr);
void		error(char *str);
void		error_msg(char *str);
void		print_struct(t_fdf *ls);
void		del_struct(t_fdf *ls);
void		draw_line(t_fdf *ls);
void		print_lines(t_fdf *ls);
void		convert_row(t_fdf *ls, char **row, int r_num);
void		rows_into_array(t_fdf *ls);
void		count_save_rows(t_fdf *ls);
void		print_in_window(t_fdf *ls);
void		zoom_reset(t_fdf *ls);
void		zoom_in(t_fdf *ls);
void		zoom(t_fdf *ls, int step);
int			key_f(int key, void *ls_void);
int			test(void *ls_void);
void		make_image(t_fdf *ls);
void		make_lines(t_fdf *ls);
void		pixel_to_img(t_fdf *ls, size_t i, size_t j, int color);
void		redraw(t_fdf *ls);

#endif