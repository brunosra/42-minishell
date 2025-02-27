/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschwell <student@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 22:18:23 by bschwell          #+#    #+#             */
/*   Updated: 2025/02/18 21:05:46 by bschwell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Error Codes:
 * C error codes: https://manpages.ubuntu.com/manpages/noble/man3/sysexits.h.3head.html
 * Bash Standard Errors: https://tldp.org/LDP/abs/html/exitcodes.html
 */

# include <sysexits.h>

#ifndef	 ERRORS_MS_H
# define ERRORS_MS_H

// Bash Standard Errors
# define EX_OK				0
# define EX_GENERICERR		1
# define EX_BUILTINERR		2
# define EX_EXECERR			126
# define EX_NOTFOUNDERR		127
# define EX_EXITARGERR		128
# define EX_OUTOFRANGEERR	255

# define MSG_ERR_EXIT		"exit\nminishell: exit:"
#endif