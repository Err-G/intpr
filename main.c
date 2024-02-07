#include <stdlib.h> // malloc
#include <unistd.h> // read & write

int _strlen(char *str) {
	int len = -1;
	while (str[++len]);
	return (len);
}

void *_calloc(int m, int s) {
	void *res = malloc(m * s);
	int i = -1;
	if (!res) return (NULL);
	while (++i < m * s) ((unsigned char *)res)[i] = 0;
	return (res);
}

char *str_join(char *a, char *b) {
	char *res = NULL;
	int i = -1;
	int j = 0;
	if (!b)
		return (NULL);
	if (!a)
		a = _calloc(1, 1);
	res = _calloc(_strlen(a) + _strlen(b) + 1, 1);
	while (a[++i])
		res[i] = a[i];
	while (b[j])
		res[i++] = b[j++];
	res[i] = 0;
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
		if (buff[0] == '\n')
			break ;
		line = str_join(line, buff);
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
	return (0);
}

int main(void)
{
	char *line = get_line(0);
	int n = 0;
	while (line) {
		if (_strcmp(line, "quit") == 0 || _strcmp(line, "exit") == 0)
			break ;
		n = _atoi(line);
		_putnbr(n);
		_putstr("\n");
		line = get_line(0);
	}
	return (0);
}
