/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   env.c											  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agalleze <agalleze@student.42.fr>		  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/09/16 12:19:47 by agalleze		  #+#	#+#			 */
/*   Updated: 2025/09/17 16:49:32 by agalleze		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

int	print_env(t_env **env)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (!current->is_exported && !current->is_local)
		{
			if (current->var_value)
			{
				ft_putstr_fd(current->var_name, 1);
				write(1, "=", 1);
			}
			if (!current->var_value)
				write(1, "\n", 1);
			if (current->var_value)
			{
				ft_putstr_fd(current->var_value, 1);
				write(1, "\n", 1);
			}
			current = current->next;
		}
		else
			current = current->next;
	}
	return (0);
}
