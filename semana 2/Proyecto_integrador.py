"""
Practica S2 - Actividad 5
Proyecto Integrador: Biblioteca Digital - Huata.
Libro, Lector y Prestamo con encapsulamiento estricto (invariantes,
propiedades calculadas y validacion de precondiciones).
"""

import re
from datetime import date, timedelta


class Libro:
    _PATRON_CODIGO = re.compile(r"^BIB\d{3}$")
    _CATEGORIAS_VALIDAS = {"infantil", "general", "referencia"}
    _DIAS_PRESTAMO = {"infantil": 21, "general": 14, "referencia": 0}

    def __init__(self, codigo: str, titulo: str, autor: str, categoria: str):
        self.__codigo = None
        self.__titulo = None
        self.__autor = None
        self.__categoria = None
        self.__disponible = True

        self.codigo = codigo
        self.titulo = titulo
        self.autor = autor
        self.categoria = categoria

    # --- codigo: validado, inmutable en la practica (no se expone setter) ---
    @property
    def codigo(self) -> str:
        return self.__codigo

    @codigo.setter
    def codigo(self, cod: str) -> None:
        if not self._PATRON_CODIGO.match(cod):
            raise ValueError(f"Código de libro inválido: {cod!r} (formato BIB+3 dígitos)")
        self.__codigo = cod

    @property
    def titulo(self) -> str:
        return self.__titulo

    @titulo.setter
    def titulo(self, valor: str) -> None:
        if not valor:
            raise ValueError("El título no puede estar vacío")
        self.__titulo = valor

    @property
    def autor(self) -> str:
        return self.__autor

    @autor.setter
    def autor(self, valor: str) -> None:
        if not valor:
            raise ValueError("El autor no puede estar vacío")
        self.__autor = valor

    @property
    def categoria(self) -> str:
        return self.__categoria

    @categoria.setter
    def categoria(self, valor: str) -> None:
        if valor not in self._CATEGORIAS_VALIDAS:
            raise ValueError(f"Categoría inválida: {valor!r} (válidas: {self._CATEGORIAS_VALIDAS})")
        self.__categoria = valor

    @property
    def disponible(self) -> bool:
        return self.__disponible

    # --- Propiedad calculada (sin setter): derivada de categoria ---
    @property
    def dias_prestamo(self) -> int:
        """Días de préstamo según la categoría. 0 = no se presta (solo consulta)."""
        return self._DIAS_PRESTAMO[self.__categoria]

    # --- Métodos de negocio: verifican precondición antes de operar ---
    def prestar(self) -> None:
        if self.__categoria == "referencia":
            raise RuntimeError(f"'{self.__titulo}' es de referencia: no se presta")
        if not self.__disponible:
            raise RuntimeError(f"'{self.__titulo}' ya está prestado")
        self.__disponible = False

    def devolver(self) -> None:
        self.__disponible = True

    def __str__(self) -> str:
        estado = "disponible" if self.__disponible else "prestado"
        return f"[{self.__codigo}] {self.__titulo} ({self.__autor}) — {self.__categoria}, {estado}"


class Lector:
    _PATRON_CODIGO = re.compile(r"^LEC\d{3}$")
    _TIPOS_VALIDOS = {"estudiante", "docente", "comunidad"}
    _LIMITES_PRESTAMO = {"estudiante": 3, "docente": 3, "comunidad": 1}

    def __init__(self, codigo: str, nombre: str, tipo: str):
        self.__codigo = None
        self.__nombre = None
        self.__tipo = None
        self.__prestamos_activos = 0

        self.codigo = codigo
        self.nombre = nombre
        self.tipo = tipo

    @property
    def codigo(self) -> str:
        return self.__codigo

    @codigo.setter
    def codigo(self, cod: str) -> None:
        if not self._PATRON_CODIGO.match(cod):
            raise ValueError(f"Código de lector inválido: {cod!r} (formato LEC+3 dígitos)")
        self.__codigo = cod

    @property
    def nombre(self) -> str:
        return self.__nombre

    @nombre.setter
    def nombre(self, valor: str) -> None:
        if not valor:
            raise ValueError("El nombre no puede estar vacío")
        self.__nombre = valor

    @property
    def tipo(self) -> str:
        return self.__tipo

    @tipo.setter
    def tipo(self, valor: str) -> None:
        if valor not in self._TIPOS_VALIDOS:
            raise ValueError(f"Tipo de lector inválido: {valor!r} (válidos: {self._TIPOS_VALIDOS})")
        self.__tipo = valor

    # --- Propiedad calculada (sin setter): derivada de tipo ---
    @property
    def limite_prestamos(self) -> int:
        """Cuántos préstamos simultáneos puede tener este lector, según su tipo."""
        return self._LIMITES_PRESTAMO[self.__tipo]

    @property
    def prestamos_activos(self) -> int:
        return self.__prestamos_activos

    # --- Método de negocio: verifica precondición antes de operar ---
    def registrar_prestamo(self) -> None:
        if self.__prestamos_activos >= self.limite_prestamos:
            raise RuntimeError(
                f"{self.__nombre} ya alcanzó su límite de {self.limite_prestamos} préstamos"
            )
        self.__prestamos_activos += 1

    def registrar_devolucion(self) -> None:
        if self.__prestamos_activos > 0:
            self.__prestamos_activos -= 1

    def __str__(self) -> str:
        return f"[{self.__codigo}] {self.__nombre} ({self.__tipo}) — {self.__prestamos_activos}/{self.limite_prestamos} préstamos"


class Prestamo:
    def __init__(self, libro: Libro, lector: Lector, fecha_prestamo: date | None = None):
        self.__fecha_prestamo = None

        self.fecha_prestamo = fecha_prestamo or date.today()
        # Precondiciones de negocio antes de crear el prestamo:
        libro.prestar()
        lector.registrar_prestamo()
        self.__libro = libro
        self.__lector = lector

    @property
    def fecha_prestamo(self) -> date:
        return self.__fecha_prestamo

    @fecha_prestamo.setter
    def fecha_prestamo(self, valor: date) -> None:
        if valor > date.today():
            raise ValueError("La fecha de préstamo no puede estar en el futuro")
        self.__fecha_prestamo = valor

    @property
    def libro(self) -> Libro:
        return self.__libro

    @property
    def lector(self) -> Lector:
        return self.__lector

    # --- Propiedad calculada (sin setter): derivada de fecha_prestamo + libro.dias_prestamo ---
    @property
    def fecha_devolucion_esperada(self) -> date:
        return self.__fecha_prestamo + timedelta(days=self.__libro.dias_prestamo)

    def cerrar(self) -> None:
        self.__libro.devolver()
        self.__lector.registrar_devolucion()

    def __str__(self) -> str:
        return (f"Préstamo: {self.__libro.titulo} -> {self.__lector.nombre} "
                f"(devolver antes de {self.fecha_devolucion_esperada})")


# --- Pruebas ---
if __name__ == "__main__":
    libro = Libro("BIB001", "Los ríos profundos", "José María Arguedas", "general")
    print(libro)
    print(f"Días de préstamo: {libro.dias_prestamo}")

    lector = Lector("LEC001", "Rosa Mamani", "comunidad")
    print(lector)

    prestamo = Prestamo(libro, lector)
    print(prestamo)
    print(libro)
    print(lector)

    # Invariantes — deben lanzar ValueError o RuntimeError
    for caso, excepcion in [
        (lambda: Libro("X9", "titulo", "autor", "general"), ValueError),
        (lambda: Libro("BIB002", "", "autor", "general"), ValueError),
        (lambda: Libro("BIB002", "titulo", "autor", "rara"), ValueError),
        (lambda: Lector("X9", "nombre", "comunidad"), ValueError),
        (lambda: Lector("LEC002", "nombre", "vecino"), ValueError),
    ]:
        try:
            caso()
            print("ERROR: se esperaba una excepción y no ocurrió")
        except excepcion as ex:
            print(f"Error esperado: {ex}")

    try:
        Prestamo(libro, lector)  # libro ya prestado
    except RuntimeError as ex:
        print(f"Error esperado: {ex}")
