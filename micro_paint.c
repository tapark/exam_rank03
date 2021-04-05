#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_zone
{
	int w;
	int h;
	char b;
}				t_zone;

typedef struct	s_rec
{
	char r;
	float x;
	float y;
	float w;
	float h;
	char b;
}				t_rec;

int ft_strlen(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

int fail(char *s)
{
	write(1, s, ft_strlen(s));
	return (1);
}

char *get_zone(FILE *fp, t_zone *zone)
{
	int n;
	char *dest;

	n = fscanf(fp, "%d %d %c\n", &zone->w, &zone->h, &zone->b);
	if (n != 3)
		return (NULL);
	if (zone->w <= 0 || zone->h <= 0 || zone->w > 300 || zone->h > 300)
		return (NULL);
	if (!(dest = (char *)malloc(sizeof(char) *(zone->w * zone->h + 1))))
		return (NULL);
	n = 0;
	while (n < (zone->w * zone->h))
		dest[n++] = zone->b;
	dest[n] = '\0';
	return (dest);
}

int get_boundary(int x, int y, t_rec *rec)
{
	if (x < rec->x || rec->x + rec->w < x ||
		y < rec->y || rec->y + rec->h < y)
		return (0);
	if (x < rec->x + 1 || rec->x + rec->w - 1 < x ||
		y < rec->y + 1 || rec->y + rec->h - 1 < y)
		return (2);
	return (1);
}

void draw_rec(t_zone *zone, t_rec *rec, char *dest)
{
	int x;
	int y;
	int i;

	y = 0;
	while (y < zone->h)
	{
		x = 0;
		while (x < zone->w)
		{
			i = get_boundary(x, y, rec);
			if (i == 2 || (i == 1 && rec->r == 'R'))
				dest[y * zone->w + x] = rec->b;
			x++;
		}
		y++;
	}
}

int get_rec(FILE *fp, t_zone *zone, char *dest)
{
	int n;
	t_rec rec;

	while ((n = fscanf(fp, "%c %f %f %f %f %c\n", &rec.r, &rec.x, &rec.y, &rec.w, &rec.h, &rec.b)) == 6)
	{
		if (rec.w <= 0 || rec.h <= 0)
			return (1);
		if (rec.r == 'r' || rec.r == 'R')
			draw_rec(zone, &rec, dest);
		else
			return (1);
	}
	if (n != -1)
		return (1);
	return (0);
}

void draw_all(char *dest, t_zone *zone)
{
	int i;

	i = 0;
	while (i < zone->h)
	{
		write(1, &dest[i * zone->w], zone->w);
		write(1, "\n", 1);
		i++;
	}
}

int main(int argc, char **argv)
{
	FILE *fp;
	t_zone zone;
	char *dest;

	if (argc != 2)
		return(fail("Error: argument\n"));
	fp = fopen(argv[1], "r");
	if (fp == NULL)
		return(fail("Error: Operation file corrupted\n"));;
	dest = get_zone(fp, &zone);
	if (dest == NULL)
	{
		fclose(fp);
		return(fail("Error: Operation file corrupted\n"));
	}
	if (get_rec(fp, &zone, dest))
	{
		fclose(fp);
		free(dest);
		return(fail("Error: Operation file corrupted\n"));
	}
	draw_all(dest, &zone);
	fclose(fp);
	free(dest);
	return (0);
}