/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amazurie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/12 15:57:16 by amazurie          #+#    #+#             */
/*   Updated: 2019/04/23 16:11:01 by amazurie         ###   ########.fr       */
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

t_syml			*syml_sorted_merge(t_syml *a, t_syml *b, int i)
{
	t_syml *result;

	result = NULL;
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);
	if ((i && ft_strcmp(a->name, b->name) > 0) || (!i
			&& ft_strcmp(a->name, b->name) == 0 && a->addr < b->addr))
	{
		result = a;
		result->next = syml_sorted_merge(a->next, b, i);
		return (result);
	}
	result = b;
	result->next = syml_sorted_merge(a, b->next, i);
	return (result);
}

void			symls_merge_sort(t_syml **headref, int i)
{
	t_syml *head;
	t_syml *a;
	t_syml *b;

	head = *headref;
	if ((head == NULL) || (head->next == NULL))
		return ;
	syml_front_back_split(head, &a, &b);
	symls_merge_sort(&a, i);
	symls_merge_sort(&b, i);
	*headref = syml_sorted_merge(a, b, i);
}
