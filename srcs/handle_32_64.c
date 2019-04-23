/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_32_64.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 10:45:44 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/23 17:14:07 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

static void	free_symls(t_data *d)
{
	t_syml	*tmp;

	tmp = d->symls;
	while (tmp)
	{
		d->symls = tmp;
		tmp = d->symls->next;
		free(d->symls);
	}
	d->symls = NULL;
}

static void	free_sects(t_data *d)
{
	t_sect	*tmp;

	tmp = d->sects;
	while (tmp)
	{
		d->sects = tmp;
		tmp = d->sects->next;
		free(d->sects);
	}
	d->sects = NULL;
}

static void	do_work(t_data *d)
{
	syml_letter(d);
	symls_merge_sort(&d->symls, 1);
	symls_merge_sort(&d->symls, 0);
//	syml_rem_dup(&d->symls, d->rmdup);
	if (d->isot)
		show_mem(d);
	else
		show_res(d);
	free_sects(d);
	free_symls(d);
}

int			handle_64b(t_data *d, char *ptr)
{
	struct mach_header_64	*h;
	struct load_command		*lc;
	uint32_t				i;
	uint32_t				j;

	h = (struct mach_header_64 *)ptr;
	lc = (struct load_command *)((char *)ptr + sizeof(*h));
	d->sects = NULL;
	d->symls = NULL;
	d->is32b = 0;
	d->isppc = rev_uint64_endian(h->cputype, d->rev) == CPU_TYPE_POWERPC64;
	i = rev_uint32_endian(h->ncmds, d->rev);
	while (i--)
	{
		if (mapped_err(d, lc))
			return (1);
		if ((j = rev_uint32_endian(lc->cmd, d->rev)) == LC_SYMTAB)
			add_sym(d, lc);
		else if (j == LC_SEGMENT_64 && add_sect(d, lc))
			return (1);
		lc = (struct load_command *)((char *)lc
				+ rev_uint32_endian(lc->cmdsize, d->rev));
	}
	do_work(d);
	return (0);
}

int			handle_32b(t_data *d, char *ptr)
{
	struct mach_header		*h;
	struct load_command		*lc;
	uint32_t				i;
	uint32_t				j;

	h = (struct mach_header *)ptr;
	lc = (struct load_command *)((char *)ptr + sizeof(struct mach_header));
	d->sects = NULL;
	d->symls = NULL;
	d->is32b = 1;
	d->isppc = rev_uint32_endian(h->cputype, d->rev) == CPU_TYPE_POWERPC;
	i = rev_uint32_endian(h->ncmds, d->rev);
	while (i--)
	{
		if (mapped_err(d, lc))
			return (1);
		if ((j = rev_uint32_endian(lc->cmd, d->rev)) == LC_SYMTAB)
			add_sym(d, lc);
		else if (j == LC_SEGMENT && add_sect_32(d, lc))
			return (1);
		lc = (struct load_command *)((char *)lc
				+ rev_uint32_endian(lc->cmdsize, d->rev));
	}
	do_work(d);
	return (0);
}
