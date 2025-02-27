/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsoare <marsoare@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 16:41:18 by marsoare          #+#    #+#             */
/*   Updated: 2024/11/12 11:27:08 by marsoare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include <structs.h>

typedef enum s_token_type
{
	WORD,
	PIPE,
	APPEND,
	OUTFILE,
	INFILE,
	HEREDOC,
	AND_IF,
	OR,
	PARENTHESIS,
}	t_token_type;

typedef enum s_token_state
{
	GENERAL,
	IN_DQUOTES,
	IN_SQUOTES,
	EXPAND,
}	t_token_state;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_token_state	state;
	int				pos;
}	t_token;

//check_token_type.c
t_token_type	token_type(char *value);

//create_token_lst.c
void			tokenize_input(t_shell *shell, char *input);
int				handle_word_token(t_shell *shell, char *input, int i);
int				handle_redir(t_shell *shell, char *input, int i);
int				handle_pipe(t_shell *shell, char *input, int i);
int				handle_quotes(t_shell *shell, char *input, int i);

//del_token.c
void			del_token(void *content);

//expand_tokens.c
void			expand_tokens(t_shell *shell);

//ft_joinstrs.c
int				join_strs(t_shell *shell, char **str, char *input, int i);
int				h_quoted_str(t_shell *shell, char **str, char *input, int i);
int				h_nonquoted_str(t_shell *shell, char **str, char *input, int i);

//handle_andif.c
int				handle_andif(t_shell *shell, char *input, int i);

//handle_expand.c
char			*handle_expand(t_shell *shell, char *input, int i);
int				prcs_expansion(t_shell *shell, char **str, char *input, int i);
int				expand_unquoted(t_shell *shell, char **str, char *input, int i);
int				expand_quoted(t_shell *shell, char **str, char *input, int i);
int				expand_single(t_shell *shell, char **str, char *input, int i);

//handle_expand_utils.c
char			*ft_strjoin_char(char *str, char c);
int				ft_flag(char c, int *i, bool flag);
t_token			*create_token(t_shell *shell, char *str);

//handle_or.c
int				handle_or(t_shell *shell, char *input, int i);

//handle_parenthesis.c
int				handle_parenthesis(t_shell *shell, char *input, int i);
int				handle_closing(t_shell *shell, char *input, int i, bool valid);
bool			validate_subs(char *subs);
int				handle_opening(t_shell *shell, char *input, int i, bool valid);
int				jump_spaces(char *input, int i);

//lexer.c
void			lexer(t_shell *shell, char	*input);

//set_token_pos.c
void			set_token_pos(t_list *lst);

//tokenize_utils.c
bool			in_quotes(char *input, int i);
bool			ft_ismeta(char *str, int i);
int				set_append(t_shell *sh, t_token *new_token, char *input, int i);
int				set_simple(t_shell *sh, t_token *new_token, char *input, int i);
bool			is_expandable(char *token);

//tokenize_utils.c
int				set_hered(t_shell *sh, t_token *new_token, char *input, int i);

//tokenize_utils2.c
int				check_balance(char *input, int i);

#endif
