#include <stdlib.h> // malloc
#include <unistd.h> // read & write

int _strlen(char *str) {
	int len = -1;
	while (str[++len]);
	return (len);
}

char *_strcpy(char *a, char *b) {
	int i = -1;
	while (b[++i]) a[i] = b[i];
	a[i] = 0;
	return (a);
}

void *_calloc(int m, int s) {
	void *res = malloc(m * s);
	int i = -1;
	if (!res) return (NULL);
	while (++i < m * s) ((unsigned char *)res)[i] = 0;
	return (res);
}

char *_strdup(char *str) {
	char *res = _calloc(_strlen(str) + 1, sizeof(char));
	if (!res) return (NULL);
	_strcpy(res, str);
	return (res);
}

char *str_join(char *a, char *b) {
	if (!b) return (NULL);
	if (!a) return (_strdup(b));
	int len_a = _strlen(a);
	int len_b = _strlen(b);
	char *res = _calloc(len_a + len_b + 1, 1);
	if (!res) return (NULL);
	_strcpy(res, a);
	_strcpy(res + len_a, b);
	free(a);
	return (res);
}

char *get_line(int fd) {
	char *line = NULL;
	char buff[2] = {0};
	int len = 0;
	len = read(fd, &buff, 1);
	while (len > 0) {
		buff[1] = 0;
		line = str_join(line, buff);
		if (buff[0] == '\n')
			break ;
		len = read(fd, &buff, 1);
	}
	if (len <= 0) {
		if (line) free(line);
		return (NULL);
	}
	return (line);
}

void _putstr(char *str) {
	int i = -1;
	while (str[++i])
		write(1, &str[i], 1);
}

void _putnbr(int nbr) {
	unsigned int n = nbr;
	if (nbr < 0) {
		n = -nbr;
		write(1, "-", 1);
	}
	if (n >= 10)
		_putnbr(n / 10);
	write(1, &"0123456789"[n % 10], 1);
}

int _atoi(char *str)
{
	int i = 0;
	int sign = 1;
	unsigned int res = 0;
	if (str[i] == '-') {
		sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9') {
		res = (res * 10) + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

int _strcmp(char *a, char *b) {
	int i = 0;
	while (a[i] && b[i]) {
		if (a[i] != b[i])
			return (a[i] - b[i]);
		i++;
	}
	return (a[i] - b[i]);
}

char *_strchr(char *str, int c) {
	int i = -1;
	while (str[++i])
		if (str[i] == (char)c) return (&str[i]);
	return (NULL);
}

int _strspn(char *str, char *accept) {
	int i = 0;
	while (str[i] && _strchr(accept, str[i])) i++;
	return (i);
}

int _strcspn(char *str, char *reject) {
	int i = 0;
	while (str[i] && !_strchr(reject, str[i])) i++;
	return (i);
}

char *_strtok(char *str, char *delim) {
	static char *buf;
	int i = 0;
	if (str) buf = str;
	if (!buf) return (NULL);
	str = buf + _strspn(buf, delim);
	buf = str + _strcspn(str, delim);
	if (buf == str) {
		buf = NULL;
		return (buf);
	}
	if (*buf) {
		*buf = 0;
		buf++;
	} else
		buf = 0;
	return (str);
}

int word_count(char *str, char *delim) {
	int res = 0;
	while (*str) {
		str += _strspn(str, delim);
		if (*str) {
			str += _strcspn(str, delim);
			res++;
		}
	}
	return (res);
}

char **_split(char *str, char *delim) {
	int i = 0;
	char **res = _calloc(word_count(str, delim) + 1, sizeof(char *));
	char *tok = _strtok(str, delim);
	if (!res) return (NULL);
	while (tok) {
		res[i] = _strdup(tok);
		if (!res[i]) {
			i = -1;
			while (res[++i]) free(res[i]);
			free(res);
			return (NULL);
		}
		tok = _strtok(NULL, delim);
		i++;
	}
	res[i] = 0;
	return (res);
}

int main(void) {
	char *line = get_line(0);
	char **words = NULL;
	int i = 0;
	while (line) {
		if (_strcmp(line, "quit\n") == 0 || _strcmp(line, "exit\n") == 0)
			break ;
		words = _split(line, " \n");
		if (!words)
			break ;
		i = -1;
		while (words[++i]) {
			_putstr(words[i]);
			if (words[i + 1])
				_putstr(" : ");
			else
				_putstr(" !");
		}
		i = -1;
		while (words[++i]) free(words[i]);
		free(words);
		/*
		n = _atoi(line);
		_putnbr(n);
		*/
		_putstr("\n");
		free(line);
		line = get_line(0);
	}
	free(line);
	return (0);
}
