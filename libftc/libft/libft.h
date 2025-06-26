/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sliziard <sliziard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:54:22 by sliziard          #+#    #+#             */
/*   Updated: 2025/06/26 08:25:00 by mle-flem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdbool.h>
# include <stdint.h>

//* ft_is
int16_t		ft_isalpha(int32_t c) __attribute__ ((const));
int16_t		ft_isdigit(int32_t c) __attribute__ ((const));
int16_t		ft_isalnum(int32_t c) __attribute__ ((const));
int16_t		ft_isascii(int32_t c) __attribute__ ((const));
int16_t		ft_isprint(int32_t c) __attribute__ ((const));

//* ft_char
int16_t		ft_toupper(int32_t c) __attribute__ ((const));
int16_t		ft_tolower(int32_t c) __attribute__ ((const));
char		*ft_strchr(const char *s, int32_t c) __attribute__ ((nonnull(1)));
char		*ft_strrchr(const char *s, int32_t c) __attribute__ ((nonnull(1)));

//* ft_safe_str
size_t		ft_strlcat(char *dst, const char *src, size_t size)
			__attribute__ ((nonnull(1, 2)));
size_t		ft_strlcpy(char *dst, const char *src, size_t size)
			__attribute__ ((nonnull(1, 2)));
/**
 * @brief Compares two strings up to a given number of characters.
 * @return 
 *	- 0, if the s1 and s2 are equal

 *	- a `negative` value if s1 is less than s2

 *	- a `positive` value if s1 is greater than s2.
 */
int32_t		ft_strncmp(const char *s1, const char *s2, size_t n)
			__attribute__ ((nonnull(1, 2)));
char		*ft_strnstr(const char *big, const char *little, size_t len)
			__attribute__ ((nonnull(1, 2)));

//* ft_str
size_t		ft_strlen(const char *str) __attribute__ ((nonnull(1), pure));
int32_t		ft_strcmp(const char *s1, const char *s2)
			__attribute__ ((nonnull(1, 2)));

//* ft_str_alloc
char		*ft_strdup(const char *s)
			__attribute__ ((warn_unused_result, nonnull(1)));
char		*ft_substr(char const *s, uint32_t start, size_t len)
			__attribute__ ((warn_unused_result, nonnull(1)));
char		*ft_strjoin(char const *s1, char const *s2)
			__attribute__ ((warn_unused_result, nonnull(1, 2)));
char		*ft_strappend(char const *base, char const *suf)
			__attribute__ ((warn_unused_result));
char		*ft_strtrim(char const *s1, char const *set)
			__attribute__ ((warn_unused_result, nonnull(1, 2)));
// split
char		**ft_split(char const *s, char c)
			__attribute__ ((warn_unused_result, nonnull(1)));
char		**ft_nsplit(char const *s, char c)
			__attribute__ ((warn_unused_result, nonnull(1)));
void		ft_splitfree(char **splited, size_t end);

//* ft_str_iter
char		*ft_strmapi(char const *s, char (*f) (uint32_t, char))
			__attribute__ ((nonnull(1, 2)));
void		ft_striteri(char *s, void (*f) (uint32_t, char *))
			__attribute__ ((nonnull(1, 2)));

//* ft_mem
void		*ft_memset(void *s, int32_t c, size_t n)
			__attribute__ ((nonnull(1)));
// Don't manage overlap
void		*ft_memcpy(void *dest, const void *src, size_t n)
			__attribute__ ((nonnull(1, 2)));
// Manage overlap
void		*ft_memmove(void *dest, const void *src, size_t n)
			__attribute__ ((nonnull(1, 2)));
void		*ft_memchr(const void *s, int32_t c, size_t n)
			__attribute__ ((nonnull(1)));
int32_t		ft_memcmp(const void *s1, const void *s2, size_t n)
			__attribute__ ((nonnull(1, 2)));

//* ft_utils
void		ft_bzero(void *s, size_t n) __attribute__ ((nonnull(1)));
void		*ft_calloc(size_t nmemb, size_t size)
			__attribute__ ((warn_unused_result, alloc_size(1, 2)));
void		*ft_realloc(void *ptr, size_t og_size, size_t new_size)
			__attribute__ ((warn_unused_result, alloc_size(3)));
uint32_t	ft_abs(int32_t value) __attribute__ ((const));

//* ft_convert
int32_t		ft_atoi(const char *nptr) __attribute__ ((nonnull(1)));
int32_t		ft_satoi(char const *nptr, int32_t *error)
			__attribute__ ((nonnull(1, 2)));
char		*ft_itoa(int32_t n);
char		*ft_ltoa(int64_t n);
void		ft_itoa_str(char *dst, int32_t n);

//* ft_convert_base
int32_t		ft_atoi_base(char *str, char *base)__attribute__ ((nonnull(1, 2)));
char		*ft_itoa_base(int32_t n, char *base)
			__attribute__ ((nonnull(2)));
char		*ft_ltoa_base(int64_t n, char *base)
			__attribute__ ((nonnull(2)));
char		*ft_ulltoa_base(uint64_t n, char *base)
			__attribute__ ((nonnull(2)));

//* ft_output
void		ft_putchar_fd(char c, int32_t fd);
void		ft_putstr_fd(const char *s, int32_t fd);
void		ft_putendl_fd(const char *s, int32_t fd);
void		ft_putnbr_fd(int32_t n, int32_t fd);

#endif
