/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_otool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 13:06:38 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/15 11:45:47 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_OTOOL_H
# define FT_NM_OTOOL_H

# include "../libft/includes/libft.h"
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>
# include <mach-o/arch.h>
# include <ar.h>

typedef struct		s_syml
{
	char			type;
	char			*name;
	uint64_t		addr;
	uint32_t		nsect;
	uint32_t		desc;
	uint32_t		t;
	size_t			len;
	int				*tab;
	struct s_syml	*next;
}					t_syml;

typedef struct		s_sect
{
	char			*name;
	char			*segname;
	uint64_t		addr;
	uint64_t		size;
	char			*off;
	struct s_sect	*next;
}					t_sect;

typedef struct		s_data
{
	char			*arg;
	char			*name;
	char			*map;
	t_syml			*symls;
	t_sect			*sects;
	char			rev;
	char			*addr;
	char			is32b;
	char			isppc;
	size_t			size;
	char			isot;
	char			multi;
	char			j;
	char			u;
	char			up_u;
	char			ot;
	char			od;
	char			g;
	char			o;
}					t_data;

int					nm_otool(t_data *d, char *file);

int					handle_64b(t_data *d, char *ptr);
int					handle_32b(t_data *d, char *ptr);
int					handle_fat(t_data *d, char *ptr);
int					handle_arch(t_data *d, char *ptr, uint32_t magi);

int					handle_sect(t_data *d, struct load_command *lc);

void				show_mem(t_data *d);

int					add_sect(t_data *d, struct load_command *lc);
int					add_sect_32(t_data *d, struct load_command *lc);

int					add_sym(t_data *d, struct load_command *lc);
int					syml_letter(t_data *d);
void				syml_rem_dup(t_syml **symls);

int					mapped_err(t_data *d, void *ptr);

void				symls_merge_sort(t_syml **symls);

void				show_res(t_data *d);
void				puthexa(size_t hex);

int					check(t_data *d, void *ptr);
uint64_t			rev_uint64_endian(uint64_t val, int rev);
uint32_t			rev_uint32_endian(uint32_t val, int rev);
int					put_err(char *msg);

#endif
