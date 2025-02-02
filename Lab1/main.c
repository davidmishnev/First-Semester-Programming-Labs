#include <stdio.h>
#include <stdlib.h>

struct Params
{
	int width;
	int height;
	double **value;
	double **inverted;
};

int ExitWithError(const char *message)
{
	fprintf(stderr, "An error has occurred: %s\n", message);
	return 1;
}

int invertMatrix(double **matrix, double **inverse, int n)
{
	double **augmented = (double **)malloc(n * sizeof(double *));
	for (int i = 0; i < n; i++)
	{
		augmented[i] = (double *)malloc(2 * n * sizeof(double));
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			augmented[i][j] = matrix[i][j];
			augmented[i][j + n] = (i == j) ? 1.0 : 0.0;
		}
	}
	for (int i = 0; i < n; i++)
	{
		double diag = augmented[i][i];
		if (diag == 0.0)
		{
			int swapRow = -1;
			for (int k = i + 1; k < n; k++)
			{
				if (augmented[k][i] != 0)
				{
					swapRow = k;
					break;
				}
			}
			if (swapRow == -1)
			{
				for (int i = 0; i < n; i++)
				{
					free(augmented[i]);
				}
				free(augmented);
				return 0;
			}
			double *temp = augmented[i];
			augmented[i] = augmented[swapRow];
			augmented[swapRow] = temp;
		}
		diag = augmented[i][i];
		for (int j = 0; j < 2 * n; j++)
		{
			augmented[i][j] /= diag;
		}
		for (int j = 0; j < n; j++)
		{
			if (j != i)
			{
				double factor = augmented[j][i];
				for (int k = 0; k < 2 * n; k++)
				{
					augmented[j][k] -= factor * augmented[i][k];
				}
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			inverse[i][j] = augmented[i][j + n];
		}
	}
	for (int i = 0; i < n; i++)
	{
		free(augmented[i]);
	}
	free(augmented);
	return 1;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		return ExitWithError("Invalid number of arguments.");
	}
	FILE *file_input = fopen(argv[1], "r");
	FILE *file_output = fopen(argv[2], "w");
	if (file_input == NULL)
	{
		return ExitWithError("No such file or directory.");
	}
	struct Params matrix;
	if (fscanf(file_input, "%d %d", &matrix.width, &matrix.height) != 2)
	{
		fclose(file_input);
		return ExitWithError("Provided file input is empty.");
	}
	if (matrix.height != matrix.width)
	{
		fprintf(file_output, "no_solution\n");
		fclose(file_output);
		fclose(file_input);
		return 0;
	}
	matrix.value = (double **)malloc(matrix.height * sizeof(double *));
	matrix.inverted = (double **)malloc(matrix.height * sizeof(double *));
	for (int i = 0; i < matrix.height; i++)
	{
		matrix.inverted[i] = (double *)malloc(matrix.width * sizeof(double));
		matrix.value[i] = (double *)malloc(matrix.width * sizeof(double));
		for (int j = 0; j < matrix.width; j++)
		{
			fscanf(file_input, "%lf", &matrix.value[i][j]);
		}
	}
	fclose(file_input);
	if (!invertMatrix(matrix.value, matrix.inverted, matrix.height))
	{
		fprintf(file_output, "no_solution\n");
		fclose(file_output);
		for (int i = 0; i < matrix.height; i++)
		{
			free(matrix.value[i]);
			free(matrix.inverted[i]);
		}
		free(matrix.value);
		free(matrix.inverted);
		return 0;
	}

	fprintf(file_output, "%d %d\n", matrix.height, matrix.width);
	for (int i = 0; i < matrix.height; i++)
	{
		for (int j = 0; j < matrix.height; j++)
		{
			fprintf(file_output, "%g ", matrix.inverted[i][j]);
		}
		fprintf(file_output, "\n");
	}
	fclose(file_output);
	for (int i = 0; i < matrix.height; i++)
	{
		free(matrix.value[i]);
		free(matrix.inverted[i]);
	}
	free(matrix.value);
	free(matrix.inverted);
	return 0;
}
