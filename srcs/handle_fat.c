/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 11:11:37 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/15 12:56:12 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static int	check_same_arch(t_data *d, char *ptr)
{
	uint32_t		i;
	struct fat_arch	*a;

	i = 0;
	a = (struct fat_arch *)(ptr + sizeof(struct fat_header));
	while (i
			< rev_uint32_endian(((struct fat_header *)ptr)->nfat_arch, d->rev))
	{
		if (mapped_err(d, (void *)a))
			return (0);
		if (rev_uint32_endian(a->cputype, d->rev) == CPU_TYPE_X86_64)
			return (i);
		a = (struct fat_arch *)((char *)a + sizeof(struct fat_arch));
		i++;
	}
	return (i - 1);
}

static int	launch(t_data *d, uint32_t magi)
{
	if (d->ot && !d->multi
			&& ft_strncmp(d->map, ARMAG, SARMAG) != 0)
	{
		if (d->multi)
			ft_putchar('\n');
		ft_putstr(d->arg);
		ft_putstr(":\n");
	}
	if (magi == FAT_MAGIC || magi == FAT_CIGAM)
		return (handle_fat(d, d->map));
	else if (magi == MH_MAGIC_64 || magi == MH_CIGAM_64)
		return (handle_64b(d, d->map));
	else if (magi == MH_MAGIC || magi == MH_CIGAM)
		return (handle_32b(d, d->map));
	else if (ft_strncmp(d->map, ARMAG, SARMAG) == 0)
		return (handle_arch(d, d->map, magi));
	return (0);
}

int			handle_fat(t_data *d, char *ptr)
{
	struct fat_header	*h;
	struct fat_arch		*a;
	uint32_t			i;
	uint32_t			magi;

	h = (struct fat_header *)ptr;
	a = (struct fat_arch *)((char *)h + sizeof(h));
	if ((i = check_same_arch(d, ptr)))
		a = (struct fat_arch *)((char *)a + (i * sizeof(struct fat_arch)));
	else
		i = 0;
	while (i++ < rev_uint32_endian(h->nfat_arch, d->rev))
	{
		if (mapped_err(d, ptr) || mapped_err(d,
				(char *)ptr + rev_uint32_endian(a->offset, d->rev)))
			return (1);
		d->map = (char *)ptr + rev_uint32_endian(a->offset, d->rev);
		magi = *(uint32_t *)d->map;
		d->rev = (d->map && d->rev) ? magi == MH_CIGAM
			|| magi == MH_CIGAM_64 || magi == FAT_CIGAM : d->rev;
		if (!launch(d, magi))
			return (1);
	}
	return (0);
}
