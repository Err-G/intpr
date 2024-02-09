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

char *_strjoin(char *a, char *b) {
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

char *_getline(int fd) {
	char *line = NULL;
	char buff[2] = {0};
	int len = 0;
	len = read(fd, &buff, 1);
	while (len > 0) {
		buff[1] = 0;
		line = _strjoin(line, buff);
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

char *_strpbrk(char *str, char *accept)
{
	int i = -1;
	while (str[++i])
		if (_strchr(accept, str[i])) return (&str[i]);
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

int _wordcount(char *str, char *delim) {
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

void *_wordfree(char **str) {
	int i = -1;
	while (str[++i]) free(str[i]);
	free(str);
	return (NULL);
}

char **_split(char *str, char *delim) {
	int i = 0;
	char **res = _calloc(_wordcount(str, delim) + 1, sizeof(char *));
	char *tok = _strtok(str, delim);
	if (!res) return (NULL);
	while (tok) {
		res[i] = _strdup(tok);
		if (!res[i]) return ((char **)_wordfree(res));
		tok = _strtok(NULL, delim);
		i++;
	}
	res[i] = 0;
	return (res);
}

int _strisdigit(char *str) {
	return (_strlen(str) == _strspn(str, "0123456789"));
}

typedef struct node node;
struct node {
	node *next;
	void *data;
};

node *node_new(void *data) {
	node *res = _calloc(1, sizeof(node));
	if (!res) return (NULL);
	res->data = data;
	res->next = NULL;
	return (res);
}

node *node_getlast(node *n) {
	node *last = n;
	if (!last) return (NULL);
	while (last->next)
		last = last->next;
	return (last);
}

void node_addend(node **n, void *data) {
	node *new_n = node_new(data);
	if (!new_n) return ;
	if (!*n) {
		*n = new_n;
		return ;
	}
	node *last = node_getlast(*n);
	last->next = new_n;
}

void node_addstart(node **n, void *data) {
	node *new_n = node_new(data);
	if (!new_n) return ;
	if (!*n) {
		*n = new_n;
		return ;
	}
	new_n->next = *n;
	*n = new_n;
}

void node_delone(node *n, void (*del)(void *)) {
	if (!n) return ;
	if (del)
		del(n->data);
	free(n);
}

void node_clear(node **n, void (*del)(void *)) {
	node *next;
	if (!n) return ;
	while (*n) {
		next = (*n)->next;
		node_delone(*n, del);
		*n = next;
	}
}

void node_iter(node *n, void (*f)(void *)) {
	if (!n || !f) return ;
	while (n) {
		f(n->data);
		n = n->next;
	}
}

void node_nbrdel(void *nbr) {
	free(nbr);
}

void node_push(node **n, int data) {
	int *res = _calloc(1, sizeof(int));
	if (!res) return ;
	*res = data;
	node_addstart(n, res);
}

int node_pop(node **n) {
	if (!*n) return (0);
	int data = *((int *)(*n)->data);
	node *next = (*n)->next;
	node_delone(*n, node_nbrdel);
	*n = next;
	return (data);
}

typedef struct word word;
struct word {
	char *name;
	void (*def)(void *);
};

word *word_new(char *name, void (*def)(void *)) {
	word *res = _calloc(1, sizeof(word));
	if (!res) return (NULL);
	res->name = _strdup(name);
	if (!res->name) {
		free(res);
		return (NULL);
	}
	res->def = def;
	return (res);
}

void word_del(void *data) {
	word *w = (word *)data;
	if (!w) return ;
	free(w->name);
	free(w);
}

void word_exec(word *w, void *data) {
	if (!w || !w->def) return ;
	w->def(data);
}

int word_cmp(word *w, char *name) {
	if (!w || !w->name) return (-1);
	return (_strcmp(w->name, name));
}

word *word_find(node *dict, char *name) {
	while (dict) {
		word *w = (word *)dict->data;
		if (word_cmp(w, name) == 0) return (w);
		dict = dict->next;
	}
	return (NULL);
}

typedef struct intpr intpr;
struct intpr {
	char *input;
	node *dict;
	node *data;
	int quit;
};

void w_quit(void *p) {
	if (!p) return ;
	intpr *pr = (intpr *)p;
	_putstr("bye!\n");
	pr->quit = 1;
}

void w_add(void *p) {
	if (!p) return ;
	intpr *pr = (intpr *)p;
	int a = node_pop(&pr->data);
	int b = node_pop(&pr->data);
	node_push(&pr->data, b + a);
}

void w_print(void *p) {
	if (!p) return ;
	intpr *pr = (intpr *)p;
	node *last = node_getlast(pr->data);
	int nbr = 0;
	if (last && last->data)
		nbr = *(int *)last->data;
	_putnbr(nbr);
	_putstr("\n");
}

void w_drop(void *p) {
	if (!p) return ;
	intpr *pr = (intpr *)p;
	node_pop(&pr->data);
}

void intpr_init(intpr *pr) {
	node_addstart(&pr->dict, word_new("bye", w_quit));
	node_addstart(&pr->dict, word_new("quit", w_quit));
	node_addstart(&pr->dict, word_new("exit", w_quit));
	node_addstart(&pr->dict, word_new("+", w_add));
	node_addstart(&pr->dict, word_new(".", w_print));
	node_addstart(&pr->dict, word_new("drop", w_drop));
}

void intpr_clear(intpr *pr) {
	if (pr->data)
		node_clear(&pr->data, node_nbrdel);
	if (pr->dict)
		node_clear(&pr->dict, word_del);
}

void intpr_eval(char *input, intpr *pr) {
	pr->input = _strtok(input, " \n");
	while (pr->input) {
		if (_strisdigit(pr->input))
			node_push(&pr->data, _atoi(pr->input));
		else if (word_find(pr->dict, pr->input))
			word_exec(word_find(pr->dict, pr->input), (void *)pr);
		else {
			_putstr(pr->input);
			_putstr(" ?\n");
		}
		pr->input = _strtok(NULL, " \n");
	}
}

int main(void) {
	char *line = _getline(0);
	intpr pr = {0};
	intpr_init(&pr);
	while (line && !pr.quit) {
		intpr_eval(line, &pr);
		if (!pr.quit) {
			free(line);
			line = _getline(0);
		}
	}
	intpr_clear(&pr);
	free(line);
	return (0);
}
