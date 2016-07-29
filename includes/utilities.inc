<?php

/**
 * @file
 * Extract the version from the Javascript file.
 *
 * @return string
 *   The version string, or NULL if we couldn't find it.
 */

/**
 * Get the installed version.
 */
function islandora_openseadragon_get_installed_version() {
  $version = &drupal_static(__FUNCTION__, NULL);

  if ($version === NULL) {
    $path = libraries_get_path('openseadragon');
    $openseadragon_js = file_get_contents("$path/openseadragon.js");
    $matches = array();
    $count = preg_match('/@version\s+OpenSeadragon\s+((\d+\.){2}\d+)/i', $openseadragon_js, $matches);
    if ($count) {
      $version = $matches[1];
    }
  }

  return $version;
}

/**
 * Helper to construct the URL used to retrieve a "clip" from Djatoka.
 *
 * @param string $params
 *   A string containing the $_GET clip value to be parsed.
 * @param bool $download
 *   Whether the URL being generated is for a download link.
 *
 * @return bool|array
 *   FALSE when invalid parameters passed in, otherwise an array containing:
 *   -original_params (string): A rawurlencoded string which can be used as or
 *   appended to the URL query string.
 *   -djatoka_image_url (string): URL to retrieve the image from Djatoka.
 *   -dimensions (array): An array containing the width and height of the image.
 *   Used when rendering the image on the print page.
 */
function islandora_openseadragon_construct_clip_url($params, $download = FALSE) {
  global $base_root;
  $settings = islandora_openseadragon_get_settings();
  $decoded_params = drupal_get_query_array($params);
  $djatoka_url = $settings['djatokaServerBaseURL'];
  $djatoka_params = array(
    'url_ver' => 'Z39.88-2004',
    'svc_id' => 'info:lanl-repo/svc/getRegion',
    'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
    'svc.format' => 'image/jpeg',
  );
  // Only grab the two parameters that are expected to be there, ignore anything
  // else.
  $allowed_djatoka_param_keys = drupal_map_assoc(array(
    'rft_id',
    'svc.region',
  ));
  $allowed_djatoka_params = array_intersect_key($decoded_params, $allowed_djatoka_param_keys);
  if (empty($allowed_djatoka_params)) {
    return FALSE;
  }
  $rft_id = $decoded_params['rft_id'];
  $path = parse_url($rft_id, PHP_URL_PATH);
  // Replace the first left-most slash to get a more Drupal-y path to validate
  // against.
  $drupal_path = preg_replace('/\//', '', $path, 1);
  $item = menu_get_item($drupal_path);
  if (!$item || $item['path'] !== 'islandora/object/%/datastream/%/view') {
    return FALSE;
  }
  $djatoka_url_params = drupal_http_build_query($djatoka_params + $allowed_djatoka_params);
  // XXX: See if Djatoka is relative or absolute and handle accordingly.
  // Building up URL this way because may encounter a relative or absolute path
  // to Djatoka. Similarly, the built in Drupal functions such as URL will
  // construct URLs with the multi-site/language prefixs present. Lastly, not
  // using url_is_external as a relative Djatoka path may have two slashes
  // preceding it which it would flag falsely.
  if (!$download || strpos($djatoka_url, 'http') === 0) {
    $url = "$djatoka_url?$djatoka_url_params";
  }
  else {
    $url = "{$base_root}{$djatoka_url}?$djatoka_url_params";
  }
  $dimensions = array();
  if (isset($decoded_params['dimensions'])) {
    $dimensions = explode(',', $decoded_params['dimensions']);
    $dimensions = array(
      'width' => $dimensions[0],
      'height' => $dimensions[1],
    );
  }
  return array(
    // XXX: Using drupal_http_build_query to avoid changing the underlying
    // functionality in the solution packs that currently implement.
    'original_params' => drupal_http_build_query($allowed_djatoka_params),
    'djatoka_image_url' => $url,
    'dimensions' => $dimensions,
  );
}


/**
 * Get OpenSeadragon settings.
 *
 * @return array
 *   Configuration options for OpenSeadragon.
 */
function islandora_openseadragon_get_settings() {
  return variable_get('islandora_openseadragon_settings', array()) + array(
    'debugMode' => FALSE,
    'djatokaServerBaseURL' => '/adore-djatoka/resolver',
    'animationTime' => '1.5',
    'blendTime' => '0.1',
    'alwaysBlend' => FALSE,
    'autoHideControls' => TRUE,
    'immediateRender' => FALSE,
    'wrapHorizontal' => FALSE,
    'wrapVertical' => FALSE,
    'wrapOverlays' => FALSE,
    'panHorizontal' => TRUE,
    'panVertical' => TRUE,
    'minZoomImageRatio' => '0.8',
    'maxZoomPixelRatio' => '2',
    'visibilityRatio' => '0.5',
    'springStiffness' => '5.0',
    'imageLoaderLimit' => '5',
    'clickTimeThreshold' => '300',
    'clickDistThreshold' => '5',
    'zoomPerClick' => '2.0',
    'zoomPerScroll' => '1.2',
    'zoomPerSecond' => '2.0',
    'showNavigator' => TRUE,
    'defaultZoomLevel' => '1.0',
  );
}

/**
 * Image metadata pertinent to this Djatoka TileSources implementation.
 *
 * @param AbstractDatastream $datastream
 *   Gets the metadata for viewing the given datastream, it assumes that the
 *   datastream is an image and it's format is supported by Adore Djatoka.
 *
 * @return array
 *   An associative array with the required metadata for AdoraDjatoka tile
 *   source to be rendered.
 *   - identifier: A URL where the datastream image can be accessed.
 *   - width: The width of the image in pixels if known.
 *   - height: The height of the image in pixels if known.
 *   - tileWidth: The width of the tiles to assumed to make up each pyramid
 *     layer in pixels.
 *   - tileHeight: The height of the tiles to assumed to make up each pyramid
 *     layer in pixels.
 *   - tileOverlap: The number of pixels each tile is expected to overlap
 *     touching tiles.
 *   - minLevel: The minimum level to attempt to load.
 *   - maxLevel: The maximum level to attempt to load.
 */
function islandora_openseadragon_djatoka_tile_source(AbstractDatastream $datastream) {
  module_load_include('inc', 'islandora', 'includes/authtokens');
  $pid = $datastream->parent->id;
  $dsid = $datastream->id;
  $token = islandora_get_object_token($pid, $dsid, 2);
  return array(
    'identifier' => url("islandora/object/{$pid}/datastream/{$dsid}/view", array(
      'absolute' => TRUE,
      'query' => array(
        'token' => $token
      ),
    )),
    'width' => NULL,
    'height' => NULL,
    'tileWidth' => 256, // @todo Get from Admin settings for Djatoka.
    'tileHeight' => 256, // @todo Get from Admin settings for Djatoka.
    'tileOverlap' => 2, // @todo Get from Admin settings for Djatoka.
    'minLevel' => NULL, // @todo Get from Admin settings for Djatoka?
    'maxLevel' => NULL, // @todo Get from Admin settings for Djatoka?
  );
}

/**
 * Performs a solr query and generates overlays for matching terms from HOCR.
 *
 * @param string $pid
 *   The object in which to query against.
 *
 * @return array
 *   The overlays to render in the OpenSeadragon viewer.
 */
function islandora_openseadragon_viewer_query_solr_for_overlays($pid) {
  // Only get solr results if present and paged content solution pack is
  // enabled.
  if (empty($_GET['solr']) || module_exists('islandora_paged_content')) {
    return NULL;
  }

  $params = $_GET['solr']['params'];
  $params['fq'][] = format_string('PID:"!pid"', array(
    '!pid' => $pid,
  ));

  module_load_include('inc', 'islandora_paged_content', 'includes/highlighting');
  $highlighting_info = islandora_paged_content_perform_solr_highlighting_query($_GET['solr']['query'], $params);
  $highlights = array();

  foreach ($highlighting_info as $pid => $info) {
    if (array_key_exists('snippets', $info)) {
      foreach ($info['snippets'] as $snippet => $bounding_boxes) {
        foreach ($bounding_boxes as $bounding_box) {
          // The size on the x axis is used to calculate the proportional
          // values under the openseadragon coordinate system.
          $highlights[] = array(
            'x' => $bounding_box['left'] / $info['page']['width'],
            'y' => $bounding_box['top'] / $info['page']['width'],
            'width' => ($bounding_box['right'] - $bounding_box['left']) / $info['page']['width'],
            'height' => ($bounding_box['bottom'] - $bounding_box['top']) / $info['page']['width'],
            'className' => "islandora-openseadragon-highlight",
          );
        }
      }
    }
  }
  return $highlights;
}
