def minhafunc(x: str) -> int:
    return x


def fatorial(n: int) -> int:
    if n == 0:
        return 1
    else:
        return 2 * fatorial(n - 1)


def main():
    k: int = minhafunc("10.2")
    print(k)


if __name__ == "__main__":
    main()

