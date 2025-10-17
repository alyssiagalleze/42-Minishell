/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agalleze <agalleze@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:48:07 by agalleze          #+#    #+#             */
/*   Updated: 2025/10/17 16:22:31 by agalleze         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sa_readline_handler(int	sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = SIGINT;
	}
}

void	init_readline_signals(void)
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = sa_readline_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	sa_heredoc_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = SIGINT;
	}
}

void	init_heredoc_signals(void)
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = sa_heredoc_handler;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	sa_exec_child_handler(int	sig)
{
	if (sig == SIGINT)
	{
		exit(sig + 228);
	}
	if (sig == SIGQUIT)
	{
		exit(sig + 228);
	}
}

void	init_exec_father_signals(void)
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	init_exec_child_signals()
{
	struct sigaction	sa;
	char				*sa_ptr;
	unsigned long		i;

	i = 0;
	sa_ptr = (char *)&sa;
	while (i < sizeof(sa))
	{
		*(sa_ptr + i) = 0;
		i ++;
	}
	sa.sa_handler = sa_exec_child_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
/* // doit prendre en argument le pid du process en cours pour pouvoir le stopper sans quitter minishell 
void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigquit_handler(int signal)
{
	if (signal == SIGQUIT)
		write(1, "", 0);
}

void	eof_handler(int signal)
{
	if (signal == EOF)
		//printf("\nEOF\n");
}

void	set_signal_action_eof(void)
{
	struct	sigaction act;
	
	bzero(&act, sizeof(act));
	act.sa_handler = &eof_handler;
	sigaction(EOF, &act, NULL);
	
}

void	set_signal_action_quit(void)
{
	struct	sigaction act;
	
	bzero(&act, sizeof(act));
	act.sa_handler = &sigquit_handler;
	sigaction(SIGQUIT, &act, NULL);
	
}

void	set_signal_action(void)
{
	struct	sigaction act;
	
	bzero(&act, sizeof(act));
	act.sa_handler = &sigint_handler;
	sigaction(SIGINT, &act, NULL);
	
} */
// int	g_exit_status = 0; //TODO : global var ???

// void	sigint_handler(int sig, siginfo_t *info, void *context)
// {
// 	(void)sig;
// 	(void)info;
// 	(void)context;
// 	g_exit_status = 130; // convention bash : 128 + SIGINT
// 	write(STDOUT_FILENO, "\n", 1);
// 	rl_on_new_line();
// 	rl_replace_line("", 0);
// 	rl_redisplay();
// }

// void	init_signals(void)
// {
// 	struct sigaction	sa;

// 	sa.sa_sigaction = sigint_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = SA_SIGINFO; // pour utiliser la version à 3 arguments
// 	sigaction(SIGINT, &sa, NULL);
// 	sa.sa_handler = SIG_IGN; // ignorer complètement
// 	sigemptyset(&sa.sa_mask);
// 	sa.sa_flags = 0;
// 	sigaction(SIGQUIT, &sa, NULL);
// }

/* int main(void)
{
    char *line;

    init_signals();

    while (1)
    {
        line = readline("minishell> ");
        if (!line) // Ctrl+D => readline renvoie NULL
        {
            //printf("exit\n");
            break;
        }
        if (*line)
            add_history(line);

        //printf("Tu as tapé: %s\n", line);
        free(line);
    }
    return 0;
}
 */


 /*
 *	signaux en tty ->
 *		^C : rajoute "^C\n" a la line et propose un nouveau prompt (no history)
 * 		^D : exit (ecrit exit ?)
 * 		^\ : nothing
 *	signaux en exec ->
 *		^C : ^C + rend la main
 * 		^D : nothing
 * 		^\ : ^\Quit (core dumped)
 *	signaux en heredoc ->
 *		^C : 
 * 		^D :
 * 		^\ : nothing
 * 
 */
