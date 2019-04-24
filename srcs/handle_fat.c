/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/15 11:11:37 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/24 13:03:26 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static int	check_same_arch(t_data *d, char *ptr, int rev)
{
	uint32_t		i;
	struct fat_arch	*a;

	i = 0;
	a = (struct fat_arch *)(ptr + sizeof(struct fat_header));
	while (i
			< rev_uint32_endian(((struct fat_header *)ptr)->nfat_arch, rev))
	{
		if (mapped_err(d, (void *)a))
			return (0);
		if (rev_uint32_endian(a->cputype, rev) == CPU_TYPE_X86_64)
			return (i + 1);
		a = (struct fat_arch *)((char *)a + sizeof(struct fat_arch));
		i++;
	}
	return (0);
}

static void	put_ar_msg(struct fat_arch *a, int rev, char ot)
{
	char	*s;

	if (rev_uint32_endian(a->cputype, rev) == CPU_TYPE_POWERPC
		&& rev_uint32_endian(a->cpusubtype, rev) == CPU_SUBTYPE_POWERPC_ALL)
		s = PPC;
	else if (rev_uint32_endian(a->cputype, rev) == CPU_TYPE_POWERPC64)
		s = PPC64;
	else if (rev_uint32_endian(a->cputype, rev) == CPU_TYPE_X86)
		s = I386;
	else if (rev_uint32_endian(a->cputype, rev) == CPU_TYPE_X86_64)
		s = X86_64;
	else 
		s = ot ? "" : DEFAULT_ARCH;
	if (s[0])
	{
		ft_putstr(" (");
		if (!ot)
			ft_putstr("for ");
		ft_putstr(s);
	}
}

static int	launch(t_data *d, uint32_t magi, struct fat_arch *a, int mul, int rev)
{
	if ((!d->ot && mul) || (d->ot && !d->multi
			&& ft_strncmp(d->map, ARMAG, SARMAG) != 0))
	{
		if ((d->multi || mul == 1) && (!d->ot || !d->f))
			ft_putchar('\n');
		ft_putstr(d->arg);
		if (mul == 1)
			put_ar_msg(a, rev, d->isot);
		ft_putstr(":\n");
	}
	d->rev = magi == MH_CIGAM
		|| magi == MH_CIGAM_64 || magi == FAT_CIGAM ? 1 : 0;
	if (magi == FAT_MAGIC || magi == FAT_CIGAM)
		return (handle_fat(d, d->map, d->rev));
	else if (magi == MH_MAGIC_64 || magi == MH_CIGAM_64)
		return (handle_64b(d, d->map));
	else if (magi == MH_MAGIC || magi == MH_CIGAM)
		return (handle_32b(d, d->map));
	else if (ft_strncmp(d->map, ARMAG, SARMAG) == 0)
		return (handle_arch(d, d->map, magi));
	return (0);
}

int			handle_fat(t_data *d, char *ptr, int rev)
{
	struct fat_header	*h;
	struct fat_arch		*a;
	uint32_t			i;
	uint32_t			magi;
	int			mul;

	d->f = 1;
	d->rmdup = 0;
	h = (struct fat_header *)ptr;
	a = (struct fat_arch *)(ptr + sizeof(struct fat_header));
	if ((i = check_same_arch(d, ptr, rev)))
	{
		a = (struct fat_arch *)((char *)a + ((i - 1) * sizeof(struct fat_arch)));
		i = rev_uint32_endian(h->nfat_arch, rev) - 1;
		mul = 0;
	}
	else
		mul = rev_uint32_endian(h->nfat_arch, rev) > 1 ? 1 : 2;
	while (i++ < rev_uint32_endian(h->nfat_arch, rev))
	{
		if (mapped_err(d, a) || mapped_err(d,
				(char *)ptr + rev_uint32_endian(a->offset, rev)))
			return (1);
		d->map = (char *)ptr + rev_uint32_endian(a->offset, rev);
		magi = *(uint32_t *)d->map;
		if (d->map <= ptr || launch(d, magi, a, mul, rev))
			return (d->map <= ptr ? put_err("File error") : 1);
		a = (struct fat_arch *)((char *)a + sizeof(struct fat_arch));
		d->f = 0;
	}
	return (0);
}
