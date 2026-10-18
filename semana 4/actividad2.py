#Práctica S4 — Actividad 2: Sistema de Reportes con Composición 
import sys
from abc import ABC, abstractmethod

sys.stdout.reconfigure(encoding="utf-8")

# PARTE A — Diseño basado en HERENCIA frágil 

class Reporte:
    def generar(self, datos: dict, nombre: str) -> str:
        raise NotImplementedError


class ReporteCSV(Reporte):
    def generar(self, datos: dict, nombre: str) -> str:
        ruta = f"reportes/{nombre}.csv"
        print(f"[CSV] Exportando a {ruta}")
        return ruta


class ReportePDF(Reporte):
    def generar(self, datos: dict, nombre: str) -> str:
        ruta = f"reportes/{nombre}.pdf"
        print(f"[PDF] Exportando a {ruta}")
        return ruta

class ReporteCSVConFirma(ReporteCSV):

    def generar(self, datos: dict, nombre: str) -> str:
        ruta = super().generar(datos, nombre)
        ruta_firmada = ruta + ".signed"
        print(f"[FIRMA] Firmando {ruta} -> {ruta_firmada}")
        return ruta_firmada


# PARTE B  Diseño basado en COMPOSICIÓN flexible
class Exportador(ABC):
    @abstractmethod
    def exportar(self, contenido: str, ruta: str) -> None: ...

    @property
    @abstractmethod
    def extension(self) -> str: ...


class Firmador(ABC):
    @abstractmethod
    def firmar(self, ruta: str) -> str:
        """Retorna ruta del archivo firmado."""
        ...


class ExportadorCSV(Exportador):
    @property
    def extension(self) -> str:
        return ".csv"

    def exportar(self, contenido: str, ruta: str) -> None:
        print(f"[CSV] Exportando a {ruta}{self.extension}")
        # En un sistema real: escribir el archivo


class ExportadorJSON(Exportador):
    @property
    def extension(self) -> str:
        return ".json"

    def exportar(self, contenido: str, ruta: str) -> None:
        print(f"[JSON] Exportando a {ruta}{self.extension}")


class FirmadorDigital(Firmador):
    def __init__(self, clave: str):
        self._clave = clave

    def firmar(self, ruta: str) -> str:
        ruta_firmada = ruta + ".signed"
        print(f"[FIRMA] Firmando {ruta} -> {ruta_firmada}")
        return ruta_firmada


class GeneradorReporte:
    """Depende de abstracciones — no de implementaciones concretas."""

    def __init__(self, exportador: Exportador, firmador: Firmador | None = None):
        self._exportador = exportador
        self._firmador = firmador

    # ---- inyección de dependencia en tiempo de ejecución ---------------
    def cambiar_exportador(self, exp: Exportador) -> None:
        self._exportador = exp

    def generar(self, datos: dict, nombre: str) -> str:
        contenido = self._formatear(datos)
        ruta_base = f"reportes/{nombre}"
        self._exportador.exportar(contenido, ruta_base)
        ruta_final = ruta_base + self._exportador.extension
        if self._firmador:
            ruta_final = self._firmador.firmar(ruta_final)
        return ruta_final

    def _formatear(self, datos: dict) -> str:
        return ", ".join(f"{k}={v}" for k, v in datos.items())


# Mock para pruebas unitarias sin tocar el sistema de archivos real
class ExportadorMock(Exportador):
    @property
    def extension(self) -> str:
        return ".mock"

    def __init__(self):
        self.llamadas = []

    def exportar(self, contenido: str, ruta: str) -> None:
        self.llamadas.append((contenido, ruta))


# ── Demostración ─────────────────────────────────────────────────────
print("=" * 65)
print("PARTE A — HERENCIA (frágil)")
print("=" * 65)
datos_planilla = {"total_personal": 45, "monto_total": 185000.50}

rep_csv = ReporteCSV()
rep_csv.generar(datos_planilla, "planilla_agosto")

rep_csv_firmado = ReporteCSVConFirma()
rep_csv_firmado.generar(datos_planilla, "planilla_firmada")
# Si ahora quisiera un PDF firmado, necesito OTRA clase: ReportePDFConFirma

print("\n" + "=" * 65)
print("PARTE B — COMPOSICIÓN (flexible)")
print("=" * 65)

# Reporte CSV sin firma
gen = GeneradorReporte(ExportadorCSV())
ruta = gen.generar(datos_planilla, "planilla_agosto")
print(f"Generado: {ruta}")

# Cambiar exportador en tiempo de ejecución — sin tocar GeneradorReporte
gen.cambiar_exportador(ExportadorJSON())
ruta = gen.generar(datos_planilla, "planilla_agosto")
print(f"Generado: {ruta}")

# Reporte CSV con firma digital
gen_firmado = GeneradorReporte(ExportadorCSV(), FirmadorDigital("clave_unap_2026"))
ruta = gen_firmado.generar(datos_planilla, "planilla_firmada")
print(f"Generado y firmado: {ruta}")

print("\n" + "=" * 65)
print("PRUEBA UNITARIA CON MOCK (sin escribir archivos reales)")
print("=" * 65)
mock = ExportadorMock()
reporte_test = GeneradorReporte(mock)
reporte_test.generar(datos_planilla, "test")
assert len(mock.llamadas) == 1
print(f"Mock registró {len(mock.llamadas)} llamada(s) -> prueba unitaria OK, "
      "sin tocar el disco")
