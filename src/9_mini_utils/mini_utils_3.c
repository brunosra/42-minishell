/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcosta-f <tcosta-f@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 06:57:01 by tcosta-f          #+#    #+#             */
/*   Updated: 2025/03/08 07:52:28 by tcosta-f         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_check_terminal_scroll(void);

void	ft_check_terminal_scroll(void)
{
	int				term_rows;
	int				cursor_row;
	struct termios	old_term;
	struct termios	new_term;

	tcgetattr(STDIN_FILENO, &old_term);
	new_term = old_term;
	new_term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	term_rows = tgetnum("li");
	cursor_row = tgetnum("co");
	if (cursor_row >= term_rows - 1)
		rl_on_new_line();
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
}
