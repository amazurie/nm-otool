/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 15:57:16 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/05 10:43:46 by amazurie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_nm_otool.h"

t_syml			*do_swap(t_syml *prec, t_syml *syml, t_syml *next)
{
	if (prec)
		prec->next = next;
	syml->next = next->next;
	next->next = syml;
	return ((prec) ? NULL : next);
}

void			sort_symls(t_syml **symls)
{
	t_syml *tmp;
	t_syml *ptmp;
	int	i;

	if (!symls || !*symls || !(*symls)->next)
		return ;
	tmp = *symls;
	ptmp = NULL;
	while (tmp->next)
	{
		//TODO add hex value check
		if ((i = ft_strcmp(tmp->name, tmp->next->name)) > 0
				|| (i == 0 && tmp->addr > tmp->next->addr))
		{
			tmp = do_swap(ptmp, tmp, tmp->next);
			if (tmp)
				*symls = tmp;
			ptmp = NULL;
			tmp = *symls;
			continue;
		}
		ptmp = tmp;
		tmp = tmp->next;
	}
}

static void		syml_front_back_split(t_syml *source,
		t_syml **frontref, t_syml **backref)
{
	t_syml *fast;
	t_syml *slow;

	slow = source;
	fast = source->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*frontref = source;
	*backref = slow->next;
	slow->next = NULL;
}

t_syml			*syml_sorted_merge(t_syml *a, t_syml *b)
{
	t_syml *result;

	result = NULL;
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if (ft_strcmp(a->name, b->name) < 0)
	{
		result = a;
		result->next = syml_sorted_merge(a->next, b);
		return (result);
	}
	result = b;
	result->next = syml_sorted_merge(a, b->next);
	return (result);
}

void			symls_merge_sort(t_syml **headref)
{
	t_syml *head;
	t_syml *a;
	t_syml *b;

	head = *headref;
	if ((head == NULL) || (head->next == NULL))
		return ;
	syml_front_back_split(head, &a, &b);
	symls_merge_sort(&a);
	symls_merge_sort(&b);
	*headref = syml_sorted_merge(a, b);
}
