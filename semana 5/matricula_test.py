# ── tests/test_matricula.py ─────────────────────────────────────────────
import pytest
from datetime import date
from matricula import Matricula
from excepciones import ErrorCapacidad, ErrorValidacion, ErrorAcademico

HOY = date.today()

def test_constructor_valido():
    m = Matricula("20210500", ["SIS206", "SIS207"], HOY)
    assert m._cursos == ["SIS206", "SIS207"]
    assert not m._anulada

def test_constructor_sin_cursos():
    with pytest.raises(ErrorCapacidad):
        Matricula("20210500", [], HOY)

def test_constructor_demasiados_cursos():
    with pytest.raises(ErrorCapacidad):
        Matricula("20210500", [f"SIS{i}" for i in range(7)], HOY)

def test_constructor_duplicados():
    with pytest.raises(ErrorValidacion):
        Matricula("20210500", ["SIS206", "SIS206"], HOY)

def test_agregar_curso_exitoso():
    m = Matricula("20210500", ["SIS206"], HOY)
    m.agregar_curso("SIS207")
    assert "SIS207" in m._cursos
    assert len(m._cursos) == 2

def test_invariante_directo():
    m = Matricula("20210500", ["SIS206"], HOY)
    m._verificar_invariante()    # no debe lanzar
