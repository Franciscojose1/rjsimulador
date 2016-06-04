<?php
namespace Drupal\rjsimulador\Renderers;

interface Renderable {
  /**
   * Permite pintar los datos del elemento como HTML.
   *
   * Genera un array de renderizado de Drupal para poder ser añadido al render array de una página cualquiera.
   *
   * @return array Render array de elemento.
   */
  public function renderableArray();
}