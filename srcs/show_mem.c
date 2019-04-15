/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_mem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/25 15:53:35 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/15 10:34:43 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

size_t		hexlen(size_t n)
{
	size_t	i;
	size_t	b;

	i = 1;
	b = n;
	while ((b /= 16) > 0)
		i++;
	return (i);
}

void		outfill(char c, uint64_t n, size_t len)
{
	size_t	nlen;
	size_t	res;

	nlen = hexlen(n);
	res = (nlen > len) ? nlen - len : len - nlen;
	while (res--)
		ft_putchar(c);
}

static void	put_mem(t_sect *s, char isppc, char is32)
{
	size_t	i;

	i = 0;
	while (i < s->size)
	{
		if (i % 16 == 0)
		{
			outfill('0', s->addr, isppc || is32 ? 8 : 16);
			(!s->addr) ? ft_putchar('0') : puthexa(s->addr);
			ft_putchar('\t');
		}
		if (!((u_char *)s->off)[i])
			ft_putchar('0');
		if (((u_char *)s->off)[i] < 16)
			ft_putchar('0');
		puthexa(((u_char *)s->off)[i]);
		i++;
		s->addr += 1;
		if (!isppc)
			ft_putchar(' ');
		else if (i % 4 == 0)
			ft_putchar(' ');
		if (i % 16 == 0 && i > 0 && i < s->size)
			ft_putchar('\n');
	}
}

void		show_mem(t_data *d)
{
	t_sect	*tmp;

	tmp = d->sects;
	while (tmp)
	{
		ft_putstr("Contents of (");
		ft_putstr(tmp->segname);
		ft_putchar(',');
		ft_putstr(tmp->name);
		ft_putstr(") section");
		if (tmp->size > 0)
			ft_putchar('\n');
		put_mem(tmp, d->isppc, d->is32b);
		tmp = tmp->next;
	}
}
