/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syml.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 11:39:39 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/09 10:45:43 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

void		syml_rem_dup(t_syml **symls)
{
	t_syml	*syml;
	t_syml	*tmp;

	if (!symls || !*symls)
		return ;
	syml = *symls;
	tmp = NULL;
	while (syml && syml->next)
	{
		while (syml && ft_strlen(syml->name) == 0)
		{
			if (!tmp)
			{
				tmp = syml;
				syml = syml->next;
				free(tmp->name);
				free(tmp);
				*symls = syml;
				tmp = NULL;
				continue;
			}
			syml = syml->next;
			free(tmp->next->name);
			free(tmp->next);
			tmp->next = syml;
		}
		//TODO test first
		while (syml->next && syml->next->type == 'U'
				&& ft_strcmp(syml->name, syml->next->name) == 0)
		{
			tmp = syml->next->next;
			free(syml->next->name);
			free(syml->next);
			syml->next = tmp;
		}
		tmp = syml;
		if (syml)
			syml = syml->next;
	}
	syml = *symls;
}

void		add_syml(t_syml **symls, t_syml *add)
{
	t_syml *tmp;

	if (!symls || !add)
		return ;
	if (!*symls)
	{
		*symls = add;
		return ;
	}
	tmp = *symls;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = add;
	return ;
}

static void	load_elem(t_data *d, struct nlist_64 *array,
		char *strtab, uint32_t i)
{
	t_syml	*tmp;

	tmp = (t_syml *)ft_memalloc(sizeof(t_syml));
	tmp->addr = rev_uint64_endian(array[i].n_value, d->rev);
	tmp->nsect = array[i].n_sect;
	tmp->t = array[i].n_type;
	tmp->desc = array[i].n_desc;
	tmp->tab = (int *)&(array[i]);
	//TODO store len then replace ndup with pustr_len
	tmp->name = ft_strndup(strtab
			+ rev_uint32_endian(array[i].n_un.n_strx, d->rev),
			d->size - (size_t)((strtab
			+ rev_uint32_endian(array[i].n_un.n_strx, d->rev)) - d->addr));
	tmp->next = NULL;
	add_syml(&d->symls, tmp);
}

static void	load_elem32(t_data *d, struct nlist *array,
		char *strtab, uint32_t i)
{
	t_syml	*tmp;

	tmp = (t_syml *)ft_memalloc(sizeof(t_syml));
	tmp->addr = rev_uint32_endian(array[i].n_value, d->rev);
	tmp->nsect = array[i].n_sect;
	tmp->t = array[i].n_type;
	tmp->desc = array[i].n_desc;
	tmp->tab = (int *)&(array[i]);
	//TODO store len then replace ndup with pustr_len
	tmp->name = ft_strndup(strtab
			+ rev_uint32_endian(array[i].n_un.n_strx, d->rev),
			d->size - (size_t)((strtab
			+ rev_uint32_endian(array[i].n_un.n_strx, d->rev)) - d->addr));
	tmp->next = NULL;
	add_syml(&d->symls, tmp);
}

int			add_sym(t_data *d, struct load_command *lc)
{
	struct symtab_command	*s;
	void					*array;
	char					*strtab;
	uint32_t				i;

	s = (struct symtab_command *)lc;
	array = (char *)d->map + rev_uint32_endian(s->symoff, d->rev);
	strtab = d->map + rev_uint32_endian(s->stroff, d->rev);
	i = -1;
	while (++i < rev_uint32_endian(s->nsyms, d->rev))
	{
		if (d->is32b)
			load_elem32(d, array, strtab, i);
		else
			load_elem(d, array, strtab, i);
	}
	return (0);
}
