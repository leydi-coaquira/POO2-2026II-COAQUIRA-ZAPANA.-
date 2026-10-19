# ── test_dominio.py ──────────────────────────────────────────────────────
# Practica S5, Actividad 5 - 6 pruebas de contratos sobre el Proyecto
# Integrador (Biblioteca Digital Huata): 3 validas, 3 que deben lanzar.
# Mismos casos que tests_dominio.cpp.
import pytest

from proyecto_integrador import LibroFisico, LibroDigital, Lector, Prestamo, buscar_en_catalogo
from excepciones import ErrorValidacion, ErrorEstado


# ── Validas (no deben lanzar) ────────────────────────────────────────────
def test_valida_1_libro_fisico_ok():
    libro = LibroFisico("BIB001", "Los rios profundos", "Jose Maria Arguedas", "Estante A-3")
    assert libro.disponible
    assert libro.dias_prestamo() == 14


def test_valida_2_prestamo_ok():
    libro = LibroDigital("BIB002", "Manual de agricultura andina", "MINAGRI", "PDF", 4.2)
    lector = Lector("LEC001", "Rosa Mamani", "comunidad")
    Prestamo(libro, lector)
    assert not libro.disponible


def test_valida_3_buscar_en_catalogo_result_ok():
    catalogo = [LibroFisico("BIB003", "Cuentos andinos", "Jose Maria Arguedas", "Estante B-1")]

    encontrado = buscar_en_catalogo(catalogo, "BIB003")
    assert encontrado.es_exito()

    no_encontrado = buscar_en_catalogo(catalogo, "BIB999")
    assert no_encontrado.es_error()


# ── Deben lanzar ──────────────────────────────────────────────────────────
def test_lanza_1_codigo_invalido():
    with pytest.raises(ErrorValidacion):
        LibroFisico("X9", "titulo", "autor", "Estante A-1")


def test_lanza_2_prestamo_duplicado():
    libro = LibroFisico("BIB004", "Todas las sangres", "Jose Maria Arguedas", "Estante A-2")
    lector = Lector("LEC002", "Juan Quispe", "estudiante")
    Prestamo(libro, lector)
    with pytest.raises(ErrorEstado):
        Prestamo(libro, lector)  # el material ya esta prestado


def test_lanza_3_lector_tipo_invalido():
    with pytest.raises(ErrorValidacion):
        Lector("LEC003", "Carlos Rojas", "vecino")  # tipo no valido

