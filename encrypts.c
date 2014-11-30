#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

char		*ft_itoa(int n);

char	*ft_strdup(const char *s1)
{
	char	*s2;
	int		a;
	size_t	i;

	i = 0;
	while (s1 && s1[i])
		i++;
	s2 = (char *)malloc(sizeof(char) * (i + 1));
	a = 0;
	while (s1 && s1[a])
	{
		s2[a] = s1[a];
		a++;
	}
	s2[a] = '\0';
	return (s2);
}

void	puterr(char *s)
{
	while (*s && write(2, s++, 1))
		;
}

int	main(int ac, char **av)
{
	int		in;
	int		end;
	char		*tmp;
	int		out;
	int		byte;
	char		*val;
	struct	stat	s;
	int		value;

	if (ac != 5)
	{
		puterr("Use : ");
		puterr(av[0]);
		puterr(" file_to_encrypt byte value c|d\n");
		puterr("byte : alter value every byte bytes\n");
		puterr("value : alter with value \n\t(use '-' at the beginning of value to use a password)");
		puterr("c : crypt\n");
		puterr("d : decrypt\n");
		return (1);
	}
	in = open(av[1], O_RDONLY);
	if (in < 0)
	{
		puterr("Could not read file\n");
		return (2);
	}
	if (av[4][0] == 'c')
		tmp = "ENCRYPTED";
	else if (av[4][0] == 'd')
		tmp = "UNCRYPTED";
	else
	{
		puterr("Wrong value for last arg\n");
		return (3);
	}
	out = open(tmp, O_WRONLY | O_CREAT, S_IRWXU);
	byte = atoi(av[2]);
	if (byte < 0)
	{
		puterr("byte shall be at least 0");
		return (4);
	}
	if (av[3][0] == '-')
	{
		val = &av[3][1];
		value = 0;
	}
	else
	{
		val = NULL;
		value = atoi(av[3]);
		if (value == 0)
			value = av[1][0];
	}
	tmp = malloc(byte + 1);
	end = 0;
	fstat(in, &s);
	while (read(in, tmp, byte + 1))
	{
		if (val)
		{
			if (av[4][0] == 'c')
				tmp[byte] += val[value];
			if (av[4][0] == 'd')
				tmp[byte] -= val[value];
			value = val[value + 1] ? value + 1 :  0;
		}
		else
			tmp[byte] = av[4][0] == 'c' ? tmp[byte] + value: tmp[byte] - value;
		end += write(out, tmp, byte + 1);
		puterr(ft_itoa((float)((float)end / (float)s.st_size) * 100));
	}
	puterr("\nDone");
	close(in);
	close(out);
	return (0);
}

char		*ft_itoa(int n)
{
	char 	*ret;
	char	rev[35];
	int	i;
	int	j;
	int	jt;
	int	nt;

	i = 9;
	rev[0] = 8;
	rev[1] = 8;
	rev[2] = 8;
	rev[3] = 32;
	rev[4] = 32;
	rev[5] = 32;
	rev[6] = 8;
	rev[7] = 8;
	rev[8] = 8;
	if (n < 0)
	{
		n *= -1;
		rev[9] = '-';
		i++;
	}
	while (n >= 0)
	{
		nt = n;
		j = 0;
		while (nt >= 10)
		{
			nt = nt / 10;
			j++;
		}
		rev[i] = nt + '0';
		i++;
		jt = j;
		while (j > 0)
		{
			rev[i + j - 1] = '0';
			nt = nt * 10;
			j--;
		}
		if (n >= 10)
			n -= nt;
		if (n < 10)
		{
			rev[i + jt - 1] = n + '0';
			n = -1;
		}
		rev[i + jt] = '\0';
	}
	ret = ft_strdup(rev);
	return (ret);
}
