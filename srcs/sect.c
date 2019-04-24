/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sect.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 11:06:37 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/24 13:09:53 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static void	add_sect2(t_sect **sects, char *name, t_sect *tmp)
{
	t_sect	*lst;

	tmp->name = name;
	tmp->next = NULL;
	if (!*sects)
		*sects = tmp;
	else
	{
		lst = *sects;
		while (lst->next)
			lst = lst->next;
		lst->next = tmp;
	}
}

static void	add_sect2_64(t_data *d, struct section_64 *s)
{
	t_sect		*tmp;

	if (!(tmp = (t_sect *)ft_memalloc(sizeof(t_sect))))
		return ;
	tmp->segname = s->segname;
	tmp->addr = rev_uint64_endian(s->addr, d->rev);
	tmp->size = rev_uint64_endian(s->size, d->rev);
	tmp->off = d->map + rev_uint32_endian(s->offset, d->rev);
	if (tmp->off + tmp->size <= tmp->off)
	{
		free(tmp);
		return ;
	}
	add_sect2(&d->sects, s->sectname, tmp);
}

static void	add_sect2_32(t_data *d, struct section *s)
{
	t_sect		*tmp;

	if (!(tmp = (t_sect *)ft_memalloc(sizeof(t_sect))))
		return ;
	tmp->segname = s->segname;
	tmp->addr = rev_uint32_endian(s->addr, d->rev);
	tmp->size = rev_uint32_endian(s->size, d->rev);
	tmp->off = d->map + rev_uint32_endian(s->offset, d->rev);
	if (tmp->off + tmp->size <= tmp->off)
	{
		free(tmp);
		return ;
	}
	add_sect2(&d->sects, s->sectname, tmp);
}

int			add_sect(t_data *d, struct load_command *lc)
{
	struct section_64	*s;
	t_sect				*tmp;
	uint32_t			i;

	s = (struct section_64 *)((char *)lc + sizeof(struct segment_command_64));
	i = rev_uint32_endian(((struct segment_command_64 *)lc)->nsects, d->rev);
	while (i--)
	{
		if (!d->isot)
		{
			if ((tmp = (t_sect *)ft_memalloc(sizeof(t_sect))))
				add_sect2(&d->sects, s->sectname, tmp);
		}
		else if (d->ot &&
				ft_strcmp(s->segname, SEG_TEXT) == 0 &&
				ft_strcmp(s->sectname, SECT_TEXT) == 0)
			add_sect2_64(d, s);
		else if (d->od &&
				ft_strcmp(s->segname, SEG_DATA) == 0 &&
				ft_strcmp(s->sectname, SECT_DATA) == 0)
			add_sect2_64(d, s);
		s++;
	}
	return (0);
}

int			add_sect_32(t_data *d, struct load_command *lc)
{
	struct section	*s;
	t_sect			*tmp;
	uint32_t		i;

	s = (struct section *)((char *)lc + sizeof(struct segment_command));
	i = rev_uint32_endian(((struct segment_command *)lc)->nsects, d->rev);
	while (i--)
	{
		if (!d->isot)
		{
			if ((tmp = (t_sect *)ft_memalloc(sizeof(t_sect))))
				add_sect2(&d->sects, s->sectname, tmp);
		}
		else if (d->ot &&
				ft_strcmp(s->segname, SEG_TEXT) == 0 &&
				ft_strcmp(s->sectname, SECT_TEXT) == 0)
			add_sect2_32(d, s);
		else if (d->od &&
				ft_strcmp(s->segname, SEG_DATA) == 0 &&
				ft_strcmp(s->sectname, SECT_DATA) == 0)
			add_sect2_32(d, s);
		s++;
	}
	return (0);
}
