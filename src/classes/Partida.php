<?php

class Partida implements ServicesAdapterInterface
{
  /* ********************************************************************************* */
  /*                                      PROPERTIES                                   */
  /* ********************************************************************************* */
  /* @var int $id_partida */
  private $id_partida;
  /* @var int $uid */
  private $uid;
  /* @var int $fecha */
  private $fecha;
  /* @var int $id_simulacion */
  private $id_simulacion;
  /* @var string $nombre_simulacion */
  private $nombre_simulacion;
  /* @var float $consumo_medio */
  private $consumo_medio;
  /* @var float $consumo_total */
  private $consumo_total;
  /* @var float $tiempo_total */
  private $tiempo_total;
  /* @var ListaInfracciones $listaInfracciones */
  private $listaInfracciones;
  /*  @var ListaDatosInstantaneos $datos */
  private $listaDatos;

  /* ********************************************************************************* */
  /*                                     CONSTRUCTOR                                   */
  /* ********************************************************************************* */
  function __construct($uid, $fecha, $id_simulacion)
  {
    $this->setUid($uid);
    $this->setFecha($fecha);
    $this->setIdSimulacion($id_simulacion);
  }


  /* ********************************************************************************* */
  /*                                      ACCESSORS                                    */
  /* ********************************************************************************* */
  /**
   * @return ListaDatosInstantaneos
   */
  public function getListaDatos()
  {
    if(!isset($this->listaDatos)) {
      $provider = FactoryDataProvider::createDataProvider();
      $this->listaDatos = $provider->loadListaDatosByPartida($this);
    }
    return $this->listaDatos;
  }

  /**
   * @return int Fecha en formato UNIX.
   */
  public function getFecha()
  {
    return $this->fecha;
  }

  /**
   * @return DateTime Fecha como un DateTime.
   */
  public function getFechaAsObject()
  {
    return (new DateTime())->setTimestamp($this->fecha);
  }

  /**
   * @param int $fecha Fecha en tiempo UNIX
   * @throws InvalidArgumentException
   */
  public function setFecha($fecha)
  {
    if (is_numeric($fecha)) {
      $this->fecha = $fecha;
    } else {
      throw new InvalidArgumentException("La fecha se tiene que pasar convertida a tiempo UNIX.");
    }
  }

  /**
   * @return int
   */
  public function getIdPartida()
  {
    return $this->id_partida;
  }

  /**
   * @param int $id_partida
   * @throws InvalidArgumentException
   */
  public function setIdPartida($id_partida)
  {
    if (is_numeric($id_partida)) {
      $this->id_partida = $id_partida;
    } else {
      throw new InvalidArgumentException("El ID de la Partida debe ser un entero");
    }
  }

  /**
   * @return int
   */
  public function getIdSimulacion()
  {
    return $this->id_simulacion;
  }

  /**
   * @param int $id_simulacion
   * @throws InvalidArgumentException
   */
  public function setIdSimulacion($id_simulacion)
  {
    if (is_numeric($id_simulacion)) {
      $this->id_simulacion = $id_simulacion;
    } else {
      throw new InvalidArgumentException("El ID de la Simulación debe ser un entero.");
    }
  }

  /**
   * @return ListaInfracciones
   */
  public function getListaInfracciones()
  {
    if (!isset($this->listaInfracciones)) {
      $provider = FactoryDataProvider::createDataProvider();
      $this->listaInfracciones = $provider->loadListaInfraccionesByPartida($this);
    }
    return $this->listaInfracciones;
  }

  /**
   * @return float
   */
  public function getConsumoMedio()
  {
    return $this->consumo_medio;
  }

  /**
   * @param float $consumo_medio
   * @throws InvalidArgumentException
   */
  public function setConsumoMedio($consumo_medio)
  {
    if (is_numeric($consumo_medio)) {
      $this->consumo_medio = $consumo_medio;
    } else {
      throw new InvalidArgumentException("El Consumo Medio " . $consumo_medio . " tiene que ser un número decimal.");
    }
  }

  /**
   * @return float
   */
  public function getConsumoTotal()
  {
    return $this->consumo_total;
  }

  /**
   * @param float $consumo_total
   * @throws InvalidArgumentException
   */
  public function setConsumoTotal($consumo_total)
  {
    if (is_numeric($consumo_total)) {
      $this->consumo_total = $consumo_total;
    } else {
      throw new InvalidArgumentException("El Consumo Total tiene que ser un número decimal.");
    }
  }

  /**
   * @return float
   */
  public function getTiempoTotal()
  {
    return $this->tiempo_total;
  }

  /**
   * @param float $tiempo_total
   * @throws InvalidArgumentException
   */
  public function setTiempoTotal($tiempo_total)
  {
    if (is_numeric($tiempo_total)) {
      $this->tiempo_total = $tiempo_total;
    } else {
      throw new InvalidArgumentException("El Tiempo Total de la simulación tiene que ser un número decimal.");
    }
  }

  /**
   * @return string
   */
  public function getNombreSimulacion()
  {
    if(!isset($this->nombre_simulacion)) {
      $provider = FactoryDataProvider::createDataProvider();
      $this->setNombreSimulacion($provider->getNombreSimulacionFromID($this->getIdSimulacion()));
    }

    return $this->nombre_simulacion;
  }

  /**
   * @param string $nombre_simulacion
   */
  private function setNombreSimulacion($nombre_simulacion)
  {
    $this->nombre_simulacion = $nombre_simulacion;
  }

  /**
   * @return int
   */
  public function getUid()
  {
    return $this->uid;
  }

  /**
   * @param int $uid
   * @throws InvalidArgumentException
   */
  public function setUid($uid)
  {
    if (is_numeric($uid)) {
      $this->uid = $uid;
    } else {
      throw new InvalidArgumentException("El UID tiene que ser un entero.");
    }
  }

  /* ********************************************************************************* */
  /*                                      METHODS                                      */
  /* ********************************************************************************* */
  /**
   * Devuelve la URL para acceder a una partida.
   * @param string|null $type Si es "html_link" se devuelve la URL como un enlace HTML.
   * @return string URL para ver los datos de una partida.
   */
  public function getURLToPartidaPage($type = null)
  {
    $url = base_path() . 'simulaciones/' . $this->getIdSimulacion() . '/partidas/' . $this->getIdPartida();

    if (isset($type) && $type == 'html_link') {
      return '<a href="' .$url. '">Ver partida</a>';
    }

    return $url;
  }

  /*
   * Guarda la partida en almacenamiento persistente.
   * @throws Exception Cuando ocurre un error durante el almacenamiento.
   */
  public function save() {
    if ($this->getUid() == null || $this->getFecha() == null || $this->getIdSimulacion() == null) {
      throw new Exception("Los campos UID, Fecha e ID de Simulación son necesarios para insertar una nueva partida");
    }

    $saver = FactoryDataSaver::createDataSaver();
    $saver->savePartida($this);
  }

  /**
   * Carga una partida con sus datos del almacenamiento.
   * @param int $id_partida El id de la partida a cargar.
   * @return Partida La partida con los datos recuperados.
   * @throws Exception Si se produce un error al cargar la partida.
   */
  public static function loadById($id_partida) {
    if (is_numeric($id_partida)) {
      $provider = FactoryDataProvider::createDataProvider();
      return $provider->loadPartidaById($id_partida);
    } else {
      throw new Exception("El id de la partida no es un entero.");
    }
  }

  /**
   * @inheritdoc
   */
  public function convertPropertiesToArray()
  {
    $partida = get_object_vars($this);

    foreach ($this->getListaInfracciones() as $key => $infraccion) {
      $partida['infracciones'][$key] = $infraccion->convertPropertiesToArray();
    }

    foreach ($this->getListaDatos() as $key => $dato) {
      $partida['datos'][$key] = $dato->convertPropertiesToArray();
    }

    return $partida;
  }
}