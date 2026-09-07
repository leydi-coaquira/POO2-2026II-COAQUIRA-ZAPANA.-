import pytest
from gestor_academico_refactor import Estudiante, RepositorioEstudiantes, GeneradorReporte


def test_registrar_y_recuperar_estudiante_por_codigo():
    repo = RepositorioEstudiantes()
    repo.registrar(Estudiante("E001", "Ana Quispe"))
    encontrado = repo.obtener("E001")
    assert encontrado.codigo == "E001"
    assert encontrado.nombre == "Ana Quispe"


def test_agregar_notas_en_limites_validos_0_y_20():
    e = Estudiante("E001", "Ana Quispe")
    e.registrar_nota(0)
    e.registrar_nota(20)
    assert [n.valor for n in e.notas] == [0, 20]


def test_calcular_promedio_con_tres_notas_conocidas():
    e = Estudiante("E001", "Ana Quispe")
    for v in (12, 15, 10):
        e.registrar_nota(v)
    assert e.promedio == pytest.approx((12 + 15 + 10) / 3)


def test_promedio_es_cero_sin_notas():
    e = Estudiante("E003", "Rosa Huanca")
    assert e.promedio == 0


def test_aprobacion_en_el_limite_10_5():
    e = Estudiante("E001", "Ana Quispe")
    e.registrar_nota(10.5)
    assert e.esta_aprobado()


def test_no_aprueba_justo_debajo_del_limite():
    e = Estudiante("E001", "Ana Quispe")
    e.registrar_nota(10.49)
    assert not e.esta_aprobado()


def test_rechaza_nota_fuera_de_rango_bajo():
    e = Estudiante("E002", "Luis Mamani")
    with pytest.raises(ValueError):
        e.registrar_nota(-0.1)


def test_rechaza_nota_fuera_de_rango_alto():
    e = Estudiante("E002", "Luis Mamani")
    with pytest.raises(ValueError):
        e.registrar_nota(20.1)


def test_rechaza_codigo_de_estudiante_duplicado():
    repo = RepositorioEstudiantes()
    repo.registrar(Estudiante("E001", "Ana Quispe"))
    with pytest.raises(ValueError):
        repo.registrar(Estudiante("E001", "Ana Quispe (duplicado)"))


def test_generar_reporte_no_altera_la_entidad():
    repo = RepositorioEstudiantes()
    e = Estudiante("E001", "Ana Quispe")
    e.registrar_nota(12)
    repo.registrar(e)
    promedio_antes = e.promedio
    GeneradorReporte.generar_texto(repo.listar())
    GeneradorReporte.generar_csv(repo.listar())
    assert e.promedio == promedio_antes