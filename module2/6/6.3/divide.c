int divide(double a, double b, double *result)
{
    if (b == 0.0) {
        return -1;
    }

    *result = a / b;
    return 0;
}
