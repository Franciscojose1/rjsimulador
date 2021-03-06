<?php
namespace Drupal\rjsimulador\DAO;

use Drupal\rjsimulador\Partida;
use Drupal\rjsimulador\Infraccion;
use Drupal\rjsimulador\DatoInstantaneo;

/**
 * Interface DataSaver Interfaz que deben implementar las clases para guardar datos.
 */
interface DataSaver {
  /**
   * Almacena una partida de forma persistente.
   *
   * @param Partida $partida El objeto a almacenar.
   */
  public function savePartida(Partida $partida);

  /**
   * Almacena un infracción de forma persistente.
   *
   * @param Infraccion $infraccion El objeto a almacenar.
   */
  public function saveInfraccion(Infraccion $infraccion);

  /**
   * Almacena un dato de forma persistente.
   *
   * @param DatoInstantaneo $dato El objeto a almacenar.
   */
  public function saveDatoInstantaneo(DatoInstantaneo $dato);

  /**
   * Almacena o actualiza un tipo de infracción.
   *
   * @param array $tipoInfraccion Tipo de infracción como un array con keys
   * 'infraction_id' e 'infraction_name'. Si 'infraction_id' no existe se
   * creará un nuevo tipo de infracción.
   */
  public function saveTipoInfraccion(array $tipoInfraccion);

  /**
   * Almacena o actualiza un tipo de simulación.
   *
   * @param array $tipoSimulacion Tipo de simulación como un array con keys
   * 'simulation_id' y 'simulation_name'. Si 'simulation_id' no existe se
   * creará un nuevo tipo de simulación.
   */
  public function saveTipoSimulacion(array $tipoSimulacion);
} 