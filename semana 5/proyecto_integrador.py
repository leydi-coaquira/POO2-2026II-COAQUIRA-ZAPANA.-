"""Proyecto Integrador - Biblioteca Digital Huata
Practica S3, Actividad 5 (jerarquia de herencia) + Practica S4, Actividad 5
(interfaz INotificable e inyeccion de dependencias) + Practica S5,
Actividad 5 (jerarquia de errores y Design by Contract).
"""
import sys
from abc import ABC, abstractmethod
from datetime import date, timedelta

from excepciones import ErrorValidacion, ErrorEstado
from result import Result

try:
    sys.stdout.reconfigure(encoding="utf-8")
except AttributeError:
    pass

class MaterialBibliografico(ABC):
    """Clase base: todo lo que la biblioteca puede prestar."""

    def __init__(self, codigo: str, titulo: str, autor: str):
        self._codigo = self._validar_codigo(codigo)
        if not titulo:
            raise ErrorValidacion("El titulo no puede estar vacio")
        if not autor:
            raise ErrorValidacion("El autor no puede estar vacio")
        self._titulo = titulo
        self._autor = autor
        self._disponible = True
        self._verificar_invariante()

    @staticmethod
    def _validar_codigo(codigo: str) -> str:
        if not codigo or len(codigo) < 3:
            raise ErrorValidacion(f"Codigo de material invalido: {codigo!r}")
        return codigo

    @property
    def codigo(self) -> str:
        return self._codigo

    @property
    def titulo(self) -> str:
        return self._titulo

    @property
    def disponible(self) -> bool:
        return self._disponible

    def marcar_prestado(self) -> None:
        if not self._disponible:
            raise ErrorEstado(f"{self._titulo} ya esta prestado")
        self._disponible = False

    def marcar_devuelto(self) -> None:
        if self._disponible:
            raise ErrorEstado(f"{self._titulo} no estaba prestado, no se puede devolver")
        self._disponible = True

    @abstractmethod
    def dias_prestamo(self) -> int:
        """Cada subclase define cuantos dias dura su prestamo."""
        ...

    def _verificar_invariante(self) -> None:
        """Invariante de clase (Actividad 5, Practica S5)."""
        if not self._codigo or len(self._codigo) < 3:
            raise ErrorEstado("Invariante violado: codigo invalido")
        if not self._titulo or not self._autor:
            raise ErrorEstado("Invariante violado: titulo o autor vacio")

    def __str__(self) -> str:
        estado = "disponible" if self._disponible else "prestado"
        return f"[{self._codigo}] {self._titulo} ({estado}, {self.dias_prestamo()} dias)"


class LibroFisico(MaterialBibliografico):
    """Un ejemplar impreso. LibroFisico ES UN MaterialBibliografico (IS-A)."""

    def __init__(self, codigo: str, titulo: str, autor: str, ubicacion_estante: str):
        super().__init__(codigo, titulo, autor)
        if not ubicacion_estante:
            raise ErrorValidacion("La ubicacion de estante no puede estar vacia")
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
            raise ErrorValidacion(f"Formato no soportado: {formato}")
        if tamano_mb <= 0:
            raise ErrorValidacion("El tamano en MB debe ser positivo")
        self._formato = formato
        self._tamano_mb = tamano_mb

    @property
    def formato(self) -> str:
        return self._formato

    def dias_prestamo(self) -> int:
        return 7  # acceso digital rota mas rapido, no sufre desgaste fisico


class Lector:
    """No pertenece a la jerarquia; se relaciona por composicion con Prestamo."""

    _TIPOS_VALIDOS = {"estudiante", "docente", "comunidad"}

    def __init__(self, codigo: str, nombre: str, tipo: str):
        self._codigo = self._validar(codigo, nombre, tipo)
        self._nombre = nombre
        self._tipo = tipo

    @staticmethod
    def _validar(codigo: str, nombre: str, tipo: str) -> str:
        if not (codigo.startswith("LEC") and len(codigo) == 6 and codigo[3:].isdigit()):
            raise ErrorValidacion(f"Codigo de lector invalido: {codigo!r} (formato LEC+3 digitos)")
        if not nombre:
            raise ErrorValidacion("El nombre del lector no puede estar vacio")
        if tipo not in Lector._TIPOS_VALIDOS:
            raise ErrorValidacion(f"Tipo de lector invalido: {tipo!r}")
        return codigo

    @property
    def codigo(self) -> str:
        return self._codigo

    @property
    def nombre(self) -> str:
        return self._nombre

    @property
    def tipo(self) -> str:
        return self._tipo

    def _verificar_invariante(self) -> None:
        """Invariante de clase (Actividad 5, Practica S5)."""
        if self._tipo not in Lector._TIPOS_VALIDOS:
            raise ErrorEstado("Invariante violado: tipo de lector invalido")

class INotificable(ABC):
    @abstractmethod
    def notificar(self, destinatario: str, mensaje: str) -> bool: ...

    @abstractmethod
    def esta_disponible(self) -> bool: ...


class NotificadorConsola(INotificable):
    """Implementacion concreta #1: simula el aviso imprimiendolo."""

    def __init__(self):
        self._activo = True

    def esta_disponible(self) -> bool:
        return self._activo

    def notificar(self, destinatario: str, mensaje: str) -> bool:
        print(f"[AVISO BIBLIOTECA -> {destinatario}]: {mensaje}")
        return True


class NotificadorMock(INotificable):
    """Implementacion concreta #2: para pruebas, no imprime nada real."""

    def __init__(self):
        self.llamadas = []

    def esta_disponible(self) -> bool:
        return True

    def notificar(self, destinatario: str, mensaje: str) -> bool:
        self.llamadas.append((destinatario, mensaje))
        return True


class Prestamo:
    """COMPOSICION: un Prestamo TIENE UN MaterialBibliografico, TIENE UN
    Lector y (desde la Semana 4) TIENE UN INotificable -- ninguno de los
    tres es un Prestamo ni Prestamo es uno de ellos (no es IS-A)."""

    def __init__(self, material: MaterialBibliografico, lector: Lector,
                 notificador: INotificable | None = None):
        self._material = material
        self._lector = lector
        self._notificador = notificador
        self._fecha_prestamo = date.today()
        material.marcar_prestado()

        if self._notificador and self._notificador.esta_disponible():
            self._notificador.notificar(
                lector.nombre,
                f"Registraste el prestamo de '{material.titulo}'. "
                f"Fecha limite: {self.fecha_devolucion_esperada}",
            )

    @property
    def fecha_devolucion_esperada(self) -> date:
        return self._fecha_prestamo + timedelta(days=self._material.dias_prestamo())

    def __str__(self) -> str:
        return (f"Prestamo: {self._material.titulo} -> {self._lector.nombre} "
                f"(devolver antes de {self.fecha_devolucion_esperada})")


def mostrar_catalogo(materiales: list[MaterialBibliografico]) -> None:
    for m in materiales:
        print(m)

def buscar_en_catalogo(catalogo: list[MaterialBibliografico],
                        codigo: str) -> Result[MaterialBibliografico, str]:
    for m in catalogo:
        if m.codigo == codigo:
            return Result.Ok(m)
    return Result.Err(f"No se encontro material con codigo '{codigo}' en el catalogo")


if __name__ == "__main__":
    catalogo: list[MaterialBibliografico] = [
        LibroFisico("BIB001", "Los rios profundos", "Jose Maria Arguedas", "Estante A-3"),
        LibroDigital("BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2),
    ]
    mostrar_catalogo(catalogo)

    lector = Lector("LEC001", "Rosa Mamani", "comunidad")
    prestamo = Prestamo(catalogo[0], lector, NotificadorConsola())
    print(prestamo)

    print("\n=== Busqueda refactorizada con Result ===")
    r_ok = buscar_en_catalogo(catalogo, "BIB002")
    if r_ok.es_exito():
        print(f"[OK] Encontrado: {r_ok.valor()}")

    r_err = buscar_en_catalogo(catalogo, "BIB999")
    if r_err.es_error():
        print(f"[Result.Err] {r_err.error()}")

    print("\n=== Errores de dominio (Design by Contract) ===")
    try:
        LibroFisico("X9", "titulo", "autor", "Estante A-1")
    except ErrorValidacion as e:
        print(f"[OK] Error esperado: {e}")

    try:
        Prestamo(catalogo[0], lector)  # ya esta prestado
    except ErrorEstado as e:
        print(f"[OK] Error esperado: {e}")