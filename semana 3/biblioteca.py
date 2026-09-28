"""Proyecto Integrador - Biblioteca Digital Huata
Practica S3, Actividad 5: jerarquia de herencia del sistema.
"""

from abc import ABC, abstractmethod
from datetime import date, timedelta


class MaterialBibliografico(ABC):
    """Clase base: todo lo que la biblioteca puede prestar."""

    def __init__(self, codigo: str, titulo: str, autor: str):
        self._codigo = self._validar_codigo(codigo)
        self._titulo = titulo
        self._autor = autor
        self._disponible = True

    @staticmethod
    def _validar_codigo(codigo: str) -> str:
        if not codigo or len(codigo) < 3:
            raise ValueError(f"Código de material inválido: {codigo!r}")
        return codigo

    @property
    def codigo(self) -> str:
        return self._codigo

    @property
    def titulo(self) -> str:
        return self._titulo

    @property
    def autor(self) -> str:
        return self._autor

    @property
    def disponible(self) -> bool:
        return self._disponible

    def marcar_prestado(self) -> None:
        if not self._disponible:
            raise RuntimeError(f"{self._titulo} ya está prestado")
        self._disponible = False

    def marcar_devuelto(self) -> None:
        self._disponible = True

    @abstractmethod
    def dias_prestamo(self) -> int:
        """Cada subclase define cuántos días dura su préstamo (metodo sobrescrito)."""
        ...

    def __str__(self) -> str:
        estado = "disponible" if self._disponible else "prestado"
        return (f"[{self._codigo}] {self._titulo} ({self._autor}) — {estado}, "
                f"{self.dias_prestamo()} días de préstamo")


class LibroFisico(MaterialBibliografico):
    """Un ejemplar impreso. LibroFisico ES UN MaterialBibliografico (IS-A)."""

    def __init__(self, codigo: str, titulo: str, autor: str, ubicacion_estante: str):
        super().__init__(codigo, titulo, autor)
        self._ubicacion_estante = ubicacion_estante

    @property
    def ubicacion_estante(self) -> str:
        return self._ubicacion_estante

    def dias_prestamo(self) -> int:
        return 14  # prestamo estandar para material impreso


class LibroDigital(MaterialBibliografico):
    """Un recurso digital. LibroDigital ES UN MaterialBibliografico (IS-A)."""

    _FORMATOS_VALIDOS = {"PDF", "EPUB"}

    def __init__(self, codigo: str, titulo: str, autor: str, formato: str, tamano_mb: float):
        super().__init__(codigo, titulo, autor)
        if formato not in self._FORMATOS_VALIDOS:
            raise ValueError(f"Formato no soportado: {formato}")
        self._formato = formato
        self._tamano_mb = tamano_mb

    @property
    def formato(self) -> str:
        return self._formato

    def dias_prestamo(self) -> int:
        return 7  # acceso digital rota mas rapido, no sufre desgaste fisico


class Lector:
    """No pertenece a la jerarquía; se relaciona por composición con Prestamo."""

    def __init__(self, codigo: str, nombre: str, tipo: str):
        self._codigo = codigo
        self._nombre = nombre
        self._tipo = tipo  # "estudiante", "docente" o "comunidad"

    @property
    def codigo(self) -> str:
        return self._codigo

    @property
    def nombre(self) -> str:
        return self._nombre


class Prestamo:
    """COMPOSICION: un Prestamo TIENE UN MaterialBibliografico y TIENE UN Lector."""

    def __init__(self, material: MaterialBibliografico, lector: Lector):
        self._material = material
        self._lector = lector
        self._fecha_prestamo = date.today()
        material.marcar_prestado()

    @property
    def fecha_devolucion_esperada(self) -> date:
        return self._fecha_prestamo + timedelta(days=self._material.dias_prestamo())

    def __str__(self) -> str:
        return (f"Préstamo: {self._material.titulo} -> {self._lector.nombre} "
                f"(devolver antes de {self.fecha_devolucion_esperada})")


def mostrar_catalogo(materiales: list[MaterialBibliografico]) -> None:
    """Itera sobre una lista HETEROGÉNEA de MaterialBibliografico: polimorfismo dinamico."""
    for m in materiales:
        print(m)  # cada objeto llama a SU PROPIO dias_prestamo() en tiempo de ejecucion


def verificar_contrato_material(m: MaterialBibliografico) -> bool:
    """Contrato LSP que toda subclase de MaterialBibliografico debe cumplir."""
    ok = isinstance(m.titulo, str) and len(m.titulo) > 0
    ok = ok and m.dias_prestamo() > 0
    ok = ok and m.codigo in str(m)
    return ok


if __name__ == "__main__":
    catalogo: list[MaterialBibliografico] = [
        LibroFisico("BIB001", "Los ríos profundos", "José María Arguedas", "Estante A-3"),
        LibroDigital("BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2),
    ]
    mostrar_catalogo(catalogo)

    print("\nVerificación LSP:")
    for m in catalogo:
        assert verificar_contrato_material(m), f"LSP violado en {m.codigo}"
    print("OK — LibroFisico y LibroDigital sustituyen completamente a MaterialBibliografico")

    print()
    lector = Lector("LEC001", "Rosa Mamani", "comunidad")
    prestamo = Prestamo(catalogo[0], lector)
    print(prestamo)