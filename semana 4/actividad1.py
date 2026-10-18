# ── Práctica S4 — Actividad 1: Sistema de Notificaciones ──────────────
import sys
from abc import ABC, abstractmethod
from typing import Protocol, runtime_checkable

sys.stdout.reconfigure(encoding="utf-8")


# Interfaz pura con ABC: solo contrato, sin estado ni implementación.
class INotificador(ABC):
    @abstractmethod
    def enviar(self, destinatario: str, mensaje: str) -> bool: ...

    @abstractmethod
    def es_disponible(self) -> bool: ...

    @property
    @abstractmethod
    def nombre(self) -> str: ...


# cualquier clase que tenga estos dos métodos cumple el protocolo,
# aunque no herede de él explícitamente.
@runtime_checkable
class ProtocolNotificador(Protocol):
    def enviar(self, destinatario: str, mensaje: str) -> bool: ...
    def es_disponible(self) -> bool: ...


class NotificadorEmail(INotificador):
    def __init__(self, servidor_smtp: str, puerto: int = 587):
        self._servidor = servidor_smtp
        self._puerto = puerto
        self._activo = True

    @property
    def nombre(self) -> str:
        return f"Email ({self._servidor})"

    def es_disponible(self) -> bool:
        return self._activo

    def enviar(self, dest: str, msg: str) -> bool:
        # Simula el envío real: imprime el mensaje y confirma éxito.
        print(f"[EMAIL -> {dest}]: {msg[:50]}...")
        return True


class NotificadorSMS(INotificador):
    MAX_CHARS = 160

    def __init__(self, proveedor: str):
        self._proveedor = proveedor
        self._activo = True

    @property
    def nombre(self) -> str:
        return f"SMS ({self._proveedor})"

    def es_disponible(self) -> bool:
        return self._activo

    def enviar(self, dest: str, msg: str) -> bool:
        if len(msg) > self.MAX_CHARS:
            raise ValueError(f"SMS excede {self.MAX_CHARS} caracteres")
        print(f"[SMS -> {dest}]: {msg}")
        return True


class SistemaAlertas:
    """Usa composición: SistemaAlertas TIENE varios INotificador,
    pero no sabe (ni le importa) qué tipo concreto de notificador usa."""

    def __init__(self, notificadores: list[INotificador]):
        if not notificadores:
            raise ValueError("Se requiere al menos un notificador")
        self._notificadores = notificadores

    def agregar_notificador(self, n: INotificador) -> None:
        self._notificadores.append(n)

    def alertar(self, dest: str, mensaje: str) -> int:
        """Envía la alerta por TODOS los canales disponibles."""
        enviados = 0
        for n in self._notificadores:
            if n.es_disponible():
                try:
                    if n.enviar(dest, mensaje):
                        enviados += 1
                except Exception as e:
                    print(f"Error en {n.nombre}: {e}")
        return enviados


# Pruebas 
sistema = SistemaAlertas([
    NotificadorEmail("smtp.unap.edu.pe"),
    NotificadorSMS("Movistar"),
])

enviados = sistema.alertar(
    "jmamani@unap.edu.pe",
    "ALERTA: Temperatura del Lago Titicaca supera 18°C"
)
print(f"\nAlerta enviada por {enviados} canales.")

# Verificar que funciona con cualquier INotificador (LSP + composición)
assert isinstance(sistema._notificadores[0], INotificador)
assert isinstance(sistema._notificadores[0], ProtocolNotificador)
print("Verificación: NotificadorEmail cumple INotificador (herencia) "
      "Y ProtocolNotificador (duck typing estructural) -> OK")