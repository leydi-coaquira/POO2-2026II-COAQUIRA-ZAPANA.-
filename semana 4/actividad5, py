"""Proyecto Integrador - Biblioteca Digital Huata
Practica S4, Actividad 5: refactor con interfaz, composicion e inyeccion
de dependencias, sobre la jerarquia de herencia hecha en la Semana 3.
"""
import sys
from abc import ABC, abstractmethod
from datetime import date, timedelta

sys.stdout.reconfigure(encoding="utf-8")


# ═══════════════════════════════════════════════════════════════════
# Jerarquía de herencia (viene de la Semana 3 — sin cambios)
# ═══════════════════════════════════════════════════════════════════
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
    def disponible(self) -> bool:
        return self._disponible

    def marcar_prestado(self) -> None:
        if not self._disponible:
            raise RuntimeError(f"{self._titulo} ya está prestado")
        self._disponible = False

    def marcar_devuelto(self) -> None:
        self._disponible = True

    @abstractmethod
    def dias_prestamo(self) -> int: ...

    def __str__(self) -> str:
        estado = "disponible" if self._disponible else "prestado"
        return f"[{self._codigo}] {self._titulo} — {estado}, {self.dias_prestamo()} días"


class LibroFisico(MaterialBibliografico):
    def __init__(self, codigo: str, titulo: str, autor: str, ubicacion_estante: str):
        super().__init__(codigo, titulo, autor)
        self._ubicacion_estante = ubicacion_estante

    def dias_prestamo(self) -> int:
        return 14


class LibroDigital(MaterialBibliografico):
    _FORMATOS_VALIDOS = {"PDF", "EPUB"}

    def __init__(self, codigo: str, titulo: str, autor: str, formato: str, tamano_mb: float):
        super().__init__(codigo, titulo, autor)
        if formato not in self._FORMATOS_VALIDOS:
            raise ValueError(f"Formato no soportado: {formato}")
        self._formato = formato

    def dias_prestamo(self) -> int:
        return 7


class Lector:
    def __init__(self, codigo: str, nombre: str, tipo: str):
        self._codigo = codigo
        self._nombre = nombre
        self._tipo = tipo

    @property
    def nombre(self) -> str:
        return self._nombre


class INotificable(ABC):
    @abstractmethod
    def notificar(self, destinatario: str, mensaje: str) -> bool: ...

    @abstractmethod
    def esta_disponible(self) -> bool: ...


class NotificadorConsola(INotificable):
    """Implementación concreta #1: simula el aviso imprimiéndolo."""

    def __init__(self):
        self._activo = True

    def esta_disponible(self) -> bool:
        return self._activo

    def notificar(self, destinatario: str, mensaje: str) -> bool:
        print(f"[AVISO BIBLIOTECA -> {destinatario}]: {mensaje}")
        return True


class NotificadorMock(INotificable):
    """Implementación concreta #2: para pruebas, no imprime nada real,
    solo registra las llamadas — útil para probar Prestamo sin generar
    salida de consola."""

    def __init__(self):
        self.llamadas = []

    def esta_disponible(self) -> bool:
        return True

    def notificar(self, destinatario: str, mensaje: str) -> bool:
        self.llamadas.append((destinatario, mensaje))
        return True



class Prestamo:
    """COMPOSICIÓN: un Prestamo TIENE UN MaterialBibliografico, TIENE UN
    Lector y (desde la Semana 4) TIENE UN INotificable — ninguno de los
    tres es un Prestamo ni Prestamo es uno de ellos (no es IS-A)."""

    def __init__(self, material: MaterialBibliografico, lector: Lector,
                 notificador: INotificable | None = None):
        # ---- inyección de dependencia: el notificador llega de afuera,
        # Prestamo NO hace algo como "self._notificador = NotificadorConsola()"
        self._material = material
        self._lector = lector
        self._notificador = notificador
        self._fecha_prestamo = date.today()
        material.marcar_prestado()

        # ANTES (Semana 3, versión frágil — solo como referencia):
        #   print(f"Notificando a {lector.nombre}...")   # <- responsabilidad
        #                                                 #    de notificación
        #                                                 #    mezclada aquí
        # AHORA: Prestamo delega esa responsabilidad al objeto inyectado.
        if self._notificador and self._notificador.esta_disponible():
            self._notificador.notificar(
                lector.nombre,
                f"Registraste el préstamo de '{material.titulo}'. "
                f"Fecha límite: {self.fecha_devolucion_esperada}"
            )

    @property
    def fecha_devolucion_esperada(self) -> date:
        return self._fecha_prestamo + timedelta(days=self._material.dias_prestamo())

    def __str__(self) -> str:
        return (f"Préstamo: {self._material.titulo} -> {self._lector.nombre} "
                f"(devolver antes de {self.fecha_devolucion_esperada})")



if __name__ == "__main__":
    libro = LibroFisico("BIB001", "Los ríos profundos", "José María Arguedas", "Estante A-3")
    lector = Lector("LEC001", "Rosa Mamani", "comunidad")

    print("=" * 65)
    print("1) Préstamo CON notificación real (NotificadorConsola inyectado)")
    print("=" * 65)
    prestamo1 = Prestamo(libro, lector, NotificadorConsola())
    print(prestamo1)

    print("\n" + "=" * 65)
    print("2) Préstamo CON mock (prueba unitaria, sin tocar la consola real)")
    print("=" * 65)
    libro2 = LibroDigital("BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2)
    mock = NotificadorMock()
    prestamo2 = Prestamo(libro2, lector, mock)
    assert len(mock.llamadas) == 1
    print(f"Mock registró {len(mock.llamadas)} llamada(s) -> se puede probar "
          f"Prestamo sin notificar a nadie de verdad.")

    print("\n" + "=" * 65)
    print("3) Préstamo SIN notificador (sigue funcionando: notificador es opcional)")
    print("=" * 65)
    libro3 = LibroFisico("BIB003", "Cuentos andinos", "José María Arguedas", "Estante B-1")
    lector3 = Lector("LEC002", "Juan Quispe", "estudiante")
    prestamo3 = Prestamo(libro3, lector3)
    print(prestamo3)
    print("OK -- Prestamo no depende de una implementación concreta de "
          "notificación, solo de la interfaz INotificable.")