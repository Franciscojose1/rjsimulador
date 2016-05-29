<?php
namespace Drupal\rjsimulador;

use Drupal\rjsimulador\Factory\FactoryDataManager;

/**
 * Class Constants Clase que almacena distintas constantes del Simulador.
 */
class Constants {
  /* var array @$tiposSimulacion  Array con todas las simulaciones de la forma id => nombreSimulacion */
  private static $tiposSimulacion;
  /* var array @$tiposSimulacion  Array con todas las simulaciones de la forma id => nombreInfraccion */
  private static $tiposInfracciones;

  public static function getTiposSimulacion() {
    if (!isset(self::$tiposSimulacion)) {
      $provider = FactoryDataManager::createDataProvider();
      self::$tiposSimulacion = $provider->loadAllTiposSimulaciones();
      return self::$tiposSimulacion;
    }

    return self::$tiposSimulacion;
  }

  /**
   * @param int $idSimulacion El ID de la Simulacion para la que recuperar el nombre.
   * @return string El nombre de la Simulacion.
   * @throws LogicException Error si no existe una Simulacion con el id pasado.
   */
  public static function getNombreSimulacion($idSimulacion) {
    if (array_key_exists($idSimulacion, self::getTiposSimulacion())) {
      return self::getTiposSimulacion()[$idSimulacion];
    } else {
      throw new LogicException(t("There is no simulation with id @id.", array('@id' => $idSimulacion)));
    }
  }

  public static function getTiposInfracciones() {
    if (!isset(self::$tiposInfracciones)) {
      $provider = FactoryDataManager::createDataProvider();
      self::$tiposInfracciones = $provider->loadAllTiposInfracciones();
      return self::$tiposInfracciones;
    }

    return self::$tiposInfracciones;
  }

  /**
   * @param int $idInfraccion El ID de la Infraccion para la que recuperar el nombre.
   * @return string El nombre de la Infraccion.
   * @throws LogicException Error si no existe una Infraccion con el id pasado.
   */
  public static function getNombreInfraccion($idInfraccion) {
    if (array_key_exists($idInfraccion, self::getTiposInfracciones())) {
      return self::getTiposInfracciones()[$idInfraccion];
    } else {
      throw new LogicException(t("There is no infraction with id @id", array('@id' => $idInfraccion)));
    }
  }
}