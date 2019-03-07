/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 12:54:05 by amazurie          #+#    #+#             */
/*   Updated: 2019/03/06 11:34:38 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static char	get_sect_type(char *sect)
{
	if (ft_strcmp(sect, SECT_TEXT) == 0)
		return ('T');
	if (ft_strcmp(sect, SECT_DATA) == 0)
		return ('D');
	if (ft_strcmp(sect, SECT_BSS) == 0)
		return ('B');
	return ('S');
}

static char	get_type(t_syml *syml, char *name)
{
	char	c;

	c = syml->t & N_TYPE;
	if (c == N_UNDF)
		return (syml->addr ? 'C' : 'U');
	if (c == N_PBUD)
		return (0);
	if (c == N_ABS)
		c = 'A';
	else if (c == N_SECT)
		c = get_sect_type(name);
	else if (c == N_INDR)
		c = 'I';
	else
		c = 0;
	if (c && !(syml->t & N_EXT))
		return (ft_tolower(c));
	return (c);
}

static char	*get_sect_name(t_sect *sects, uint32_t u)
{
	t_sect		*tmp;
	uint32_t	i;

	if (!sects || u == 0)
		return (NULL);
	tmp = sects;
	i = 0;
	while (tmp && ++i < u)
		tmp = tmp->next;
	if (!tmp)
		return (NULL);
	return (tmp->name);
}

int			syml_letter(t_data *d)
{
	t_syml	*tmp;
	char	*name;

	if (!d->symls || !d->sects)
		return (1);
	tmp = d->symls;
	while (tmp)
	{
		name = get_sect_name(d->sects, tmp->nsect);
		tmp->type = get_type(tmp, name);
		tmp = tmp->next;
	}
	return (0);
}
