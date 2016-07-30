<?php

/**
 * @file
 * Admin callbacks for Islandora OpenSeadragon
 */

/**
 * Admin form. Descriptions taken from the deep zoom module.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_openseadragon_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
  $settings = islandora_openseadragon_get_settings();
  $version = islandora_openseadragon_get_installed_version();
  $djatoka_url = islandora_system_settings_form_default_value('islandora_openseadragon_djatoka_url', 'adore-djatoka/resolver', $form_state);
  $loris_url = islandora_system_settings_form_default_value('islandora_openseadragon_loris_url', 'loris', $form_state);
  $form = array(
    'source' => array(
      '#type' => 'select',
      '#title' => t('Image Server'),
      '#description' => t('Select the image server to use with OpenSeadragon'),
      '#options' => array(
        'djatoka' => t('Adore-Djatoka Image Server'),
        'loris' => t('Loris IIIF Image Server'),
      ),
    ),
    'djatoka' => array(
      '#type' => 'fieldset',
      '#title' => t('Adore Djatoka Image Server Settings'),
      '#description' => t('<p>Settings for Adore-Djatoka Image Server</p>'),
      '#states' => array(
        'visible' => array(
          ':input[name="source"]' => array('value' => 'djatoka'),
        ),
      ),
      'islandora_openseadragon_djatoka_url' => array(
        '#prefix' => '<div id="islandora-openseadragon-djatoka-url-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => t('Adore-Djatoka Image Server Base URL'),
        '#title_display' => 'invisible',
        // @todo Move validation to form level.
        '#element_validate' => array('islandora_openseadragon_djatoka_url_validate'),
        '#default_value' => $djatoka_url,
        '#description' => t('The location of the Adore-Djatoka server OpenURL resolver. <br/> !confirmation_message', array(
          '!confirmation_message' => islandora_openseadragon_admin_form_djatoka_access_message($djatoka_url),
        )),
        '#ajax' => array(
          'callback' => 'islandora_openseadragon_admin_ajax_djatoka_url',
          'wrapper' => 'islandora-openseadragon-djatoka-url-wrapper',
        ),
      ),
    ),
    'loris' => array(
      '#type' => 'fieldset',
      '#title' => t('Loris IIIF Image Server Settings'),
      '#description' => t('Settings for Loris IIIF Image Server'),
      '#states' => array(
        'visible' => array(
          ':input[name="source"]' => array('value' => 'loris'),
        ),
      ),
      // @todo Add '*' and validate conditionally.
      'islandora_openseadragon_loris_url' => array(
        '#type' => 'textfield',
        '#title' => t('Loris IIIF Image Server Base URL'),
        '#element_validate' => array('islandora_openseadragon_loris_url_validate'),
        '#default_value' => $loris_url,
        '#description' => t('The location of the Loris IIIF Image Server. <br/> !confirmation_message', array(
          '!confirmation_message' => islandora_openseadragon_admin_form_loris_access_message($loris_url),
        )),
      ),
    ),
    'openseadragon' => array(
      '#type' => 'fieldset',
      '#title' => t('Open Seadragon Viewer settings'),
      '#description' => t('<p>Settings for OpenSeadragon %version, see the <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.Options">documentation</a> for more details.</p>', array(
        '%version' => $version,
      )),
      'islandora_openseadragon_settings' => array(
        '#type' => 'container',
        '#tree' => TRUE,
        'tabIndex' => array(
          '#type' => 'textfield',
          '#title' => t('Tab Index'),
          '#default_value' => $settings['tabIndex'],
          '#element_validate' => array('element_validate_number'),
          '#size' => 10,
          '#description' => t('Tabbing order index to assign to the viewer element. Positive values are selected in increasing order. When tabIndex is 0 source order is used. A negative value omits the viewer from the tabbing order..'),
        ),
        'debugMode' => array(
          '#type' => 'checkbox',
          '#title' => t('Debug mode'),
          '#default_value' => $settings['debugMode'],
          '#description' => t('Toggles whether messages should be logged and fail-fast behavior should be provided.'),
        ),
        'debugGridColor' => array(
          '#type' => 'textfield',
          '#title' => t('Debug Grid Color'),
          '#default_value' => $settings['debugMode'],
          '#description' => t('Color of the grid in debug mode.'),
        ),
        'blendTime' => array(
          '#type' => 'textfield',
          '#title' => t('Blend time'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['blendTime'],
          '#description' => t('Specifies the duration of animation as higher or lower level tiles are replacing the existing tile.'),
        ),
        'alwaysBlend' => array(
          '#type' => 'checkbox',
          '#title' => t('Always blend'),
          '#default_value' => $settings['alwaysBlend'],
          '#description' => t("Forces the tile to always blend. By default the tiles skip blending when the blendTime is surpassed and the current animation frame would not complete the blend.."),
        ),
        'autoHideControls' => array(
          '#type' => 'checkbox',
          '#title' => t('Auto-hide controls'),
          '#default_value' => $settings['autoHideControls'],
          '#description' => t("If the user stops interacting with the viewport, fade the navigation controls. Useful for presentation since the controls are by default floated on top of the image the user is viewing."),
        ),
        'immediateRender' => array(
          '#type' => 'checkbox',
          '#title' => t('Immediate render'),
          '#default_value' => $settings['immediateRender'],
          '#description' => t('Render the best closest level first, ignoring the lowering levels which provide the effect of very blurry to sharp. It is recommended to change setting to true for mobile devices.'),
        ),
        'defaultZoomLevel' => array(
          '#type' => 'textfield',
          '#title' => t('Default zoom level'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['defaultZoomLevel'],
          '#description' => t('Zoom level to use when image is first opened or the home button is clicked. If 0, adjusts to fit viewer.'),
        ),
        'opacity' => array(
          '#type' => 'checkbox',
          '#title' => t('Opacity'),
          '#default_value' => $settings['opacity'],
          '#description' => t('Default opacity of the tiled images (checked=opaque, unchecked=transparent)'),
        ),
        'compositeOperation' => array(
          '#type' => 'select',
          '#title' => t('Composite Operation'),
          '#description' => t('Select the image server to use with OpenSeadragon'),
          '#default_value' => $settings['compositeOperation'],
          '#options' => drupal_map_assoc(array(
            NULL,
            'source-over',
            'source-atop',
            'source-in',
            'source-out',
            'destination-over',
            'destination-atop',
            'destination-in',
            'destination-out',
            'lighter',
            'copy',
            'xor'
          )),
        ),
        // @todo placeholderFillStyle.
        'degrees' => array(
          '#type' => 'textfield',
          '#title' => t('Initial Rotation'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['degrees'],
          '#description' => t('Initial rotation in degrees.'),
        ),
        'minZoomLevel' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum Zoom Level'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minZoomLevel'],
          '#description' => t('Minimum Zoom Level (integer).'),
        ),
        'maxZoomLevel' => array(
          '#type' => 'textfield',
          '#title' => t('Maximum Zoom Level'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['maxZoomLevel'],
          '#description' => t('Maximum Zoom Level (integer).'),
        ),
        'homeFillsViewer' => array(
          '#type' => 'checkbox',
          '#title' => t('Home Button Fills Viewer'),
          '#default_value' => $settings['homeFillsViewer'],
          '#description' => t('Make the "home" button fill the viewer and clip the image, instead of fitting the image to the viewer and letterboxing.'),
        ),
        'panHorizontal' => array(
          '#type' => 'checkbox',
          '#title' => t('Pan horizontal'),
          '#default_value' => $settings['panHorizontal'],
          '#description' => t('Toggles whether images can be moved along the horizontal axis of the viewer.'),
        ),
        'panVertical' => array(
          '#type' => 'checkbox',
          '#title' => t('Pan vertical'),
          '#default_value' => $settings['panVertical'],
          '#description' => t('Toggles whether images can be moved along the vertical axis of the viewer.'),
        ),
        'constrainDuringPan' => array(
          '#type' => 'checkbox',
          '#title' => t('Constrain During Pan'),
          '#default_value' => $settings['constrainDuringPan'],
        ),
        'wrapHorizontal' => array(
          '#type' => 'checkbox',
          '#title' => t('Wrap horizontal'),
          '#default_value' => $settings['wrapHorizontal'],
          '#description' => t('Toggle to force the image to wrap horizontally within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.'),
        ),
        'wrapVertical' => array(
          '#type' => 'checkbox',
          '#title' => t('Wrap vertical'),
          '#default_value' => $settings['wrapVertical'],
          '#description' => t('Toggle to force the image to wrap vertically within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.'),
        ),
        'minZoomImageRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum zoom image ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minZoomImageRatio'],
          '#description' => t('The minimum percentage ( expressed as a number between 0 and 1 ) of the viewport height or width at which the zoom out will be constrained. Setting it to 0, for example will allow you to zoom out infinity.'),
        ),
        'maxZoomPixelRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Maximum zoom pixel ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['maxZoomPixelRatio'],
          '#description' => t('The maximum ratio to allow a zoom-in to affect the highest level pixel ratio. This can be set to Infinity to allow "infinite" zooming into the image though it is less effective visually if the HTML5 Canvas is not available on the viewing device.'),
        ),
        'smoothTileEdgesMinZoom' => array(
          '#type' => 'textfield',
          '#title' => t('Smooth Tile Edges Minimum Zoom'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['smoothTileEdgesMinZoom'],
          '#description' => t('A zoom percentage ( where 1 is 100% ) of the highest resolution level. When zoomed in beyond this value alternative compositing will be used to smooth out the edges between tiles. This will have a performance impact. Can be set to Infinity to turn it off. Note: This setting is ignored on iOS devices due to a known bug (See <a href="https://github.com/openseadragon/openseadragon/issues/952">https://github.com/openseadragon/openseadragon/issues/952</a>).'),
        ),
        // @todo iOSDevice
        'autoResize' => array(
          '#type' => 'checkbox',
          '#title' => t('Auto Resize'),
          '#default_value' => $settings['autoResize'],
          '#description' => t('Set to false to prevent polling for viewer size changes. Useful for providing custom resize behavior.'),
        ),
        'preserveImageSizeOnResize' => array(
          '#type' => 'checkbox',
          '#title' => t('Auto Resize'),
          '#default_value' => $settings['preserveImageSizeOnResize'],
          '#description' => t('Set to true to have the image size preserved when the viewer is resized. This requires autoResize=true (default).'),
        ),
        'minScrollDeltaTime' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum Scroll Delta Time'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minScrollDeltaTime'],
          '#description' => t('Number of milliseconds between canvas-scroll events. This value helps normalize the rate of canvas-scroll events between different devices, causing the faster devices to slow down enough to make the zoom control more manageable.'),
        ),
        'pixelsPerWheelLine' => array(
          '#type' => 'textfield',
          '#title' => t('Pixels Per Wheel Line'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['pixelsPerWheelLine'],
          '#description' => t('For pixel-resolution scrolling devices, the number of pixels equal to one scroll line.'),
        ),
        'visibilityRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Visibility ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['visibilityRatio'],
          '#description' => t('The percentage ( as a number from 0 to 1 ) of the source image which must be kept within the viewport. If the image is dragged beyond that limit, it will \'bounce\' back until the minimum visibility ratio is achieved. Setting this to 0 and wrapHorizontal ( or wrapVertical ) to true will provide the effect of an infinitely scrolling viewport.'),
        ),
        'imageLoaderLimit' => array(
          '#type' => 'textfield',
          '#title' => t('Image loader limit'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['imageLoaderLimit'],
          '#description' => t('The maximum number of image requests to make concurrently. By default it is set to 0 allowing the browser to make the maximum number of image requests in parallel as allowed by the browsers policy.'),
        ),
        'clickTimeThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Click time threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['clickTimeThreshold'],
          '#description' => t('The number of milliseconds within which a pointer down-up event combination will be treated as a click gesture.'),
        ),
        'clickDistThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Click distance threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['clickDistThreshold'],
          '#description' => t('The maximum distance allowed between a pointer down event and a pointer up event to be treated as a click gesture.'),
        ),
        'dblClickTimeThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Double click distance threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['dblClickTimeThreshold'],
          '#description' => t('The number of milliseconds within which two pointer down-up event combinations will be treated as a double-click gesture.'),
        ),
        'dblClickDistThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Double click distance threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['dblClickDistThreshold'],
          '#description' => t('The maximum distance allowed between two pointer click events to be treated as a double-click gesture.'),
        ),
        'springStiffness' => array(
          '#type' => 'textfield',
          '#title' => t('Spring stiffness'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['springStiffness'],
          '#description' => t('Determines how sharply the springs used for animations move.'),
        ),
        'animationTime' => array(
          '#type' => 'textfield',
          '#title' => t('Animation time'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['animationTime'],
          '#description' => t('Specifies the animation duration per each OpenSeadragon.Spring which occur when the image is dragged or zoomed.'),
        ),
        'gestureSettingsMouse' => array(
          '#type' => 'fieldset',
          '#title' => t('Mouse Pointer Gesture Settings'),
          '#description' => t('<p>Settings for gestures generated by a mouse pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsMouse']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsMouse']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsMouse']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'textfield',
            '#title' => t('Pinch Rotate'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'gestureSettingsTouch' => array(
          '#type' => 'fieldset',
          '#title' => t('Touch Pointer Gesture Settings'),
          '#description' => t('<p>Settings for gestures generated by a touch pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsTouch']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsTouch']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsTouch']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'textfield',
            '#title' => t('Pinch Rotate'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsTouch']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'gestureSettingsPen' => array(
          '#type' => 'fieldset',
          '#title' => t('Pen Pointer Gesture Settings'),
          '#description' => t('<p>Settings for gestures generated by a pen pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsPen']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'textfield',
            '#title' => t('Pinch Rotate'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'gestureSettingsUnknown' => array(
          '#type' => 'fieldset',
          '#title' => t('Unknown Pointer Gesture Settings'),
          '#description' => t('<p>Settings for gestures generated by a unknown pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsUnknown']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsUnknown']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'textfield',
            '#title' => t('Pinch Rotate'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'zoomPerClick' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per click'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['zoomPerClick'],
          '#description' => t('The "zoom distance" per mouse click or touch tap. Note: Setting this to 1.0 effectively disables the click-to-zoom feature (also see gestureSettings[Mouse|Touch|Pen].clickToZoom/dblClickToZoom).'),
        ),
        'zoomPerScroll' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per scroll'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['zoomPerScroll'],
          '#description' => t('The "zoom distance" per mouse scroll or touch pinch. Note: Setting this to 1.0 effectively disables the mouse-wheel zoom feature (also see gestureSettings[Mouse|Touch|Pen].scrollToZoom}).'),
        ),
        'zoomPerSecond' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per second'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['zoomPerSecond'],
          '#description' => t('The number of seconds to animate a single zoom event over.'),
        ),
        'showNavigator' => array(
          '#type' => 'checkbox',
          '#title' => t('Show Navigator'),
          '#default_value' => $settings['showNavigator'],
          '#description' => t('Set to true to make the navigator minimap appear.'),
        ),
        // @todo navigatorId.
        'navigatorPosition' => array(
          '#type' => 'select',
          '#title' => t('Composite Operation'),
          '#description' => t('If "ABSOLUTE" is specified, then navigator[Top|Left|Height|Width] determines the size and position of the navigator minimap in the viewer, and navigatorSizeRatio and navigatorMaintainSizeRatio are ignored. For "TOP_LEFT", "TOP_RIGHT", "BOTTOM_LEFT", and "BOTTOM_RIGHT", the navigatorSizeRatio or navigator[Height|Width] values determine the size of the navigator minimap.'),
          '#default_value' => $settings['navigatorPosition'],
          '#options' => drupal_map_assoc(array(
            'TOP_RIGHT',
            'TOP_LEFT',
            'BOTTOM_LEFT',
            'BOTTOM_RIGHT',
            'ABSOLUTE',
          )),
        ),
        'navigatorSizeRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per second'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['navigatorSizeRatio'],
          '#description' => t('Ratio of navigator size to viewer size. Ignored if navigator[Height|Width] are specified.'),
        ),
        'navigatorMaintainSizeRatio' => array(
          '#type' => 'checkbox',
          '#title' => t('Navigator Maintain Size Ration'),
          '#default_value' => $settings['navigatorMaintainSizeRatio'],
          '#description' => t('If true, the navigator minimap is resized (using navigatorSizeRatio) when the viewer size changes.'),
        ),
        'navigatorTop' => array(
          '#type' => 'textfield',
          '#title' => t('Navigator Top Position'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['navigatorTop'],
          '#description' => t('Specifies the location of the navigator minimap (see Navigator Position).'),
        ),
        'navigatorLeft' => array(
          '#type' => 'textfield',
          '#title' => t('Navigator Left Position'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['navigatorLeft'],
          '#description' => t('Specifies the location of the navigator minimap (see Navigator Position).'),
        ),
        'navigatorHeight' => array(
          '#type' => 'textfield',
          '#title' => t('Navigator Height'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['navigatorHeight'],
          '#description' => t('Specifies the size of the navigator minimap (see Navigator Position). If specified, Navigator Size Ratio and Navigator Maintain Size Ratio are ignored.'),
        ),
        'navigatorWidth' => array(
          '#type' => 'textfield',
          '#title' => t('Navigator Width'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['navigatorWidth'],
          '#description' => t('Specifies the size of the navigator minimap (see Navigator Position). If specified, Navigator Size Ratio and Navigator Maintain Size Ratio are ignored.'),
        ),
        'navigatorAutoResize' => array(
          '#type' => 'checkbox',
          '#title' => t('Navigator Auto Resize'),
          '#default_value' => $settings['navigatorAutoResize'],
          '#description' => t('Set to false to prevent polling for navigator size changes. Useful for providing custom resize behavior. Setting to false can also improve performance when the navigator is configured to a fixed size.'),
        ),
        'navigatorAutoFade' => array(
          '#type' => 'checkbox',
          '#title' => t('Navigator Auto Fade'),
          '#default_value' => $settings['navigatorAutoFade'],
          '#description' => t('If the user stops interacting with the viewport, fade the navigator minimap. Setting to false will make the navigator minimap always visible.'),
        ),
        'navigatorAutoFade' => array(
          '#type' => 'checkbox',
          '#title' => t('Navigator Auto Fade'),
          '#default_value' => $settings['navigatorAutoFade'],
          '#description' => t('If the user stops interacting with the viewport, fade the navigator minimap. Setting to false will make the navigator minimap always visible.'),
        ),
        navigatorRotate
      )
    ),
    /*
    'islandora_openseadragon_settings' => array(
      '#type' => 'container',
      '#tree' => TRUE,
      'debugMode' => array(
        '#type' => 'checkbox',
        '#title' => t('Debug mode'),
        '#default_value' => $settings['debugMode'],
        '#description' => t('Toggles whether messages should be logged and fail-fast behavior should be provided.'),
      ),
      'djatokaServerBaseURL' => array(
        '#type' => 'textfield',
        '#title' => t('Djatoka server base URL'),
        '#element_validate' => array('djatoka_base_url_validate'),
        '#default_value' => $settings['djatokaServerBaseURL'],
        '#description' => t('The location of the Djatoka server OpenURL resolver.'),
      ),
      'animationTime' => array(
        '#type' => 'textfield',
        '#title' => t('Animation time'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['animationTime'],
        '#description' => t('The amount of time in seconds that animations should last.'),
      ),

      'wrapHorizontal' => array(
        '#type' => 'checkbox',
        '#title' => t('Wrap horizontal'),
        '#default_value' => $settings['wrapHorizontal'],
        '#description' =>
        t('Toggles whether tiles should be "wrapped" horizontally, so that there are no left or right edges.') . '<br />' .
        '<strong>' . t('NOTE: this is an experimental API and is not guaranteed to work. The API is also very likely to change in the future. Use at your own risk!') . '</strong>',
      ),
      'wrapVertical' => array(
        '#type' => 'checkbox',
        '#title' => t('Wrap vertical'),
        '#default_value' => $settings['wrapVertical'],
        '#description' =>
        t('Toggles whether tiles should be "wrapped" vertically, so that there are no top or bottom edges.') . '<br />' .
        '<strong>' . t('NOTE: this is an experimental API and is not guaranteed to work. The API is also very likely to change in the future. Use at your own risk!') . '</strong>',
      ),
      'wrapOverlays' => array(
        '#type' => 'checkbox',
        '#title' => t('Wrap overlays'),
        '#default_value' => $settings['wrapOverlays'],
        '#description' =>
        t('Toggles whether overlays should be continually repositioned to match any horizontal or vertical wrapping. This has no effect if neither Wrap Horizontal nor Wrap Vertical are set.') . '<br />' .
        '<strong>' . t('NOTE: this is an experimental API and is not guaranteed to work. The API is also very likely to change in the future. Use at your own risk!') . '</strong>',
      ),
      'panHorizontal' => array(
        '#type' => 'checkbox',
        '#title' => t('Pan horizontal'),
        '#default_value' => $settings['panHorizontal'],
        '#description' => t('Toggles whether images can be moved along the horizontal axis of the viewer.'),
      ),
      'panVertical' => array(
        '#type' => 'checkbox',
        '#title' => t('Pan vertical'),
        '#default_value' => $settings['panVertical'],
        '#description' => t('Toggles whether images can be moved along the vertical axis of the viewer.'),
      ),
      'showNavigator' => array(
        '#type' => 'checkbox',
        '#title' => t('Show Navigator'),
        '#default_value' => $settings['showNavigator'],
        '#description' => t('Toggles whether image navigator is displayed.'),
      ),
      'minZoomImageRatio' => array(
        '#type' => 'textfield',
        '#title' => t('Minimum zoom image ratio'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['minZoomImageRatio'],
        '#description' => t('The minimum image ratio (image size to viewer size) in both dimensions that can result from zooming out.'),
      ),
      'maxZoomPixelRatio' => array(
        '#type' => 'textfield',
        '#title' => t('Maximum zoom pixel ratio'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['maxZoomPixelRatio'],
        '#description' => t('The maximum pixel ratio (screen pixels to image file pixels) that can result from zooming in.'),
      ),
      'visibilityRatio' => array(
        '#type' => 'textfield',
        '#title' => t('Visibility ratio'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['visibilityRatio'],
        '#description' => t('The minimum portion of the viewport that must show visible content in both dimensions.'),
      ),
      'springStiffness' => array(
        '#type' => 'textfield',
        '#title' => t('Spring stiffness'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['springStiffness'],
        '#description' => t('Determines how sharply the springs used for animations move.'),
      ),
      'imageLoaderLimit' => array(
        '#type' => 'textfield',
        '#title' => t('Image loader limit'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['imageLoaderLimit'],
        '#description' => t('The maximum number of concurrent image downloads that can be performed by each viewer.'),
      ),
      'clickTimeThreshold' => array(
        '#type' => 'textfield',
        '#title' => t('Click time threshold'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['clickTimeThreshold'],
        '#description' => t('The maximum number of milliseconds that can pass between a mousedown and a mouseup for the action to still be considered a "quick" click.'),
      ),
      'clickDistThreshold' => array(
        '#type' => 'textfield',
        '#title' => t('Click distance threshold'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['clickDistThreshold'],
        '#description' => t('The maximum number of pixels the mouse can move between a mousedown and a mouseup for the action to still be considered a "quick" click.'),
      ),
      'zoomPerClick' => array(
        '#type' => 'textfield',
        '#title' => t('Zoom per click'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['zoomPerClick'],
        '#description' => t('The factor by which images should zoom when clicked on.'),
      ),
      'zoomPerScroll' => array(
        '#type' => 'textfield',
        '#title' => t('Zoom per scroll'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['zoomPerScroll'],
        '#description' => t('The factor by which images should zoom when scrolled over.'),
      ),
      'zoomPerSecond' => array(
        '#type' => 'textfield',
        '#title' => t('Zoom per second'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['zoomPerSecond'],
        '#description' => t('The factor by which images should zoom every second when the zoom buttons are held down.'),
      ),
      'defaultZoomLevel' => array(
        '#type' => 'textfield',
        '#title' => t('Default zoom level'),
        '#size' => 10,
        '#element_validate' => array('element_validate_number'),
        '#default_value' => $settings['defaultZoomLevel'],
        '#description' => t('The zoom level used by default for loaded OpenSeadragon viewers.'),
      ),
    ),
    'islandora_openseadragon_tile_size' => array(
      '#type' => 'textfield',
      '#title' => t('Tile size'),
      '#size' => 10,
      '#element_validate' => array('element_validate_integer'),
      '#default_value' => variable_get('islandora_openseadragon_tile_size', '256'),
      '#description' => t('The size of image tiles used to compose the image as a whole, in pixels.'),
    ),
    'islandora_openseadragon_tile_overlap' => array(
      '#type' => 'textfield',
      '#title' => t('Tile overlap'),
      '#size' => 10,
      '#element_validate' => array('element_validate_integer'),
      '#default_value' => variable_get('islandora_openseadragon_tile_overlap', '0'),
      '#description' => t('The number of pixels by which each tile will overlap adjacent ones.'),
    ),
    'islandora_openseadragon_fit_to_aspect_ratio' => array(
      '#type' => 'checkbox',
      '#title' => t('Constrain image to viewport'),
      '#default_value' => variable_get('islandora_openseadragon_fit_to_aspect_ratio', FALSE),
    ),
    */
    'actions' => array(
      '#type' => 'actions',
      'reset' => array(
        '#type' => 'submit',
        '#value' => t('Reset to defaults'),
        '#weight' => 1,
        '#submit' => array('islandora_openseadragon_admin_submit'),
      ),
    ),
  );
  return system_settings_form($form);
}

/**
 * Get the element to render for the AJAX event that triggered this callback.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The form element to render.
 */
function islandora_openseadragon_admin_ajax_djatoka_url(array $form, array $form_state) {
  return $form['djatoka']['islandora_openseadragon_djatoka_url'];
}

/**
 * Gets a message which describes if Adore-Djatoka is accessible.
 *
 * @param string $path
 *   The path to the Adore Djatoka image resolver.
 *
 * @return string
 *   A message describing the accessibility of the Adore-Djatoka image resolver.
 */
function islandora_openseadragon_admin_form_djatoka_access_message($path) {
  $url = url($path, array(
    'absolute' => TRUE,
    'query' => array(
      'url_ver' => 'Z39.88-2004',
      'rft_id' => 'http://memory.loc.gov/gmd/gmd433/g4330/g4330/np000066.jp2',
      'svc_id' => 'info:lanl-repo/svc/getRegion',
      'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
      'svc.format' => 'image/jpeg',
      'svc.level' => '1',
    )
  ));
  $result = drupal_http_request($url);
  if ($result->code == 200) {
    $confirmation_message = theme_image(array('path' => 'misc/watchdog-ok.png', 'attributes' => array()));
    $confirmation_message .= t('Successfully connected to Adore Djatoka Server.');
  }
  else {
    $confirmation_message = theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()));
    $confirmation_message .= t('Unable to connect to Adore Djatoka Server at !path', array(
      '!path' => $path));
  }
  return $confirmation_message;
}

/**
 * Gets a message which describes if Loris is accessible.
 *
 * @param string $url
 *   The url to the Loris IIIF image resolver.
 *
 * @return string
 *   A message describing the accessibility of the Loris IIIF image resolver.
 */
function islandora_openseadragon_admin_form_loris_access_message($url) {
  // Work around drupal_http_request's not handling relative URLs.
  if (strpos($url, '/') === 0) {
    $url = $GLOBALS['base_root'] . $url;
  }
  // Check that it's working with LOC's stable external JP2 image.
  $result = drupal_http_request(check_url($url) . '?url_ver=Z39.88-2004&rft_id=http%3A%2F%2Fmemory.loc.gov%2Fgmd%2Fgmd433%2Fg4330%2Fg4330%2Fnp000066.jp2&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/jpeg&svc.level=1');
  if ($result->code == 200) {
    $confirmation_message = theme_image(array('path' => 'misc/watchdog-ok.png', 'attributes' => array()));
    $confirmation_message .= t('Successfully connected to Loris Server.');
  }
  else {
    $confirmation_message = theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()));
    $confirmation_message .= t('Unable to connect to Loris Server at !url', array(
      '!url' => $url));
  }
  return $confirmation_message;
}

/**
 * Validates that the supplied djatoka base URL is a working djatoka install.
 *
 * @param array $element
 *   The element to validate.
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 */
function islandora_openseadragon_djatoka_url_validate(array $element, array &$form_state, array $form) {
  $path = $element['#value'];
  if (empty($path)) {
    form_error($element, t('This field is required.'));
  }
  else {
    $url = url($path, array(
      'absolute' => TRUE,
      'query' => array(
        'url_ver' => 'Z39.88-2004',
        'rft_id' => 'http://memory.loc.gov/gmd/gmd433/g4330/g4330/np000066.jp2',
        'svc_id' => 'info:lanl-repo/svc/getRegion',
        'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
        'svc.format' => 'image/jpeg',
        'svc.level' => '1',
      )
    ));
    $result = drupal_http_request($url);
    if ($result->code != 200) {
      form_error($element, t('This does not seem to be a functioning Djatoka server.'));
    }
  }
}

/**
 * Validates that the supplied loris base URL is a working loris install.
 *
 * @param array $element
 *   The element to validate.
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 */
function islandora_openseadragon_loris_url_validate(array $element, array &$form_state, array $form) {

}

/**
 * Handles a submit from the admin pane.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 */
function islandora_openseadragon_admin_submit(array $form, array &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_openseadragon_settings');
      variable_del('islandora_openseadragon_tile_size');
      variable_del('islandora_openseadragon_tile_overlap');
      break;
  }
}

/**
name: id
type: String
attributes:
default:
description: Id of the element to append the viewer's container element to. If not provided, the 'element' property must be provided. If both the element and id properties are specified, the viewer is appended to the element provided in the element property.
name: element
type: Element
attributes:
default:
description: The element to append the viewer's container element to. If not provided, the 'id' property must be provided. If both the element and id properties are specified, the viewer is appended to the element provided in the element property.
name: tileSources
type: Array | String | function | Object
attributes:<optional>
default: null
description: Tile source(s) to open initially. This is a complex parameter; see OpenSeadragon.Viewer#open for details.
name: tabIndex
type: Number
attributes:<optional>
default: 0
description: Tabbing order index to assign to the viewer element. Positive values are selected in increasing order. When tabIndex is 0 source order is used. A negative value omits the viewer from the tabbing order.
name: overlays
type: Array
attributes:
default:
description: Array of objects defining permanent overlays of the viewer. The overlays added via this option and later removed with OpenSeadragon.Viewer#removeOverlay will be added back when a new image is opened. To add overlays which can be definitively removed, one must use OpenSeadragon.Viewer#addOverlay If displaying a sequence of images, the overlays can be associated with a specific page by passing the overlays array to the page's tile source configuration. Expected properties: * x, y, (or px, py for pixel coordinates) to define the location. * width, height in point if using x,y or in pixels if using px,py. If width and height are specified, the overlay size is adjusted when zooming, otherwise the size stays the size of the content (or the size defined by CSS). * className to associate a class to the overlay * id to set the overlay element. If an element with this id already exists, it is reused, otherwise it is created. If not specified, a new element is created. * placement a string to define the relative position to the viewport. Only used if no width and height are specified. Default: 'TOP_LEFT'. See OpenSeadragon.Placement for possible values.
name: xmlPath
type: String
attributes:<optional>
default: null
description: DEPRECATED. A relative path to load a DZI file from the server. Prefer the newer Options.tileSources.
name: prefixUrl
type: String
attributes:<optional>
default: '/images/'
description: Prepends the prefixUrl to navImages paths, which is very useful since the default paths are rarely useful for production environments.
name: navImages
type: OpenSeadragon.NavImages
attributes:<optional>
default:
description: An object with a property for each button or other built-in navigation control, eg the current 'zoomIn', 'zoomOut', 'home', and 'fullpage'. Each of those in turn provides an image path for each state of the button or navigation control, eg 'REST', 'GROUP', 'HOVER', 'PRESS'. Finally the image paths, by default assume there is a folder on the servers root path called '/images', eg '/images/zoomin_rest.png'. If you need to adjust these paths, prefer setting the option.prefixUrl rather than overriding every image path directly through this setting.
name: debugMode
type: Boolean
attributes:<optional>
default: false
description: TODO: provide an in-screen panel providing event detail feedback.
name: debugGridColor
type: String
attributes:<optional>
default: '#437AB2'
description:
name: blendTime
type: Number
attributes:<optional>
default: 0
description: Specifies the duration of animation as higher or lower level tiles are replacing the existing tile.
name: alwaysBlend
type: Boolean
attributes:<optional>
default: false
description: Forces the tile to always blend. By default the tiles skip blending when the blendTime is surpassed and the current animation frame would not complete the blend.
name: autoHideControls
type: Boolean
attributes:<optional>
default: true
description: If the user stops interacting with the viewport, fade the navigation controls. Useful for presentation since the controls are by default floated on top of the image the user is viewing.
name: immediateRender
type: Boolean
attributes:<optional>
default: false
description: Render the best closest level first, ignoring the lowering levels which provide the effect of very blurry to sharp. It is recommended to change setting to true for mobile devices.
name: defaultZoomLevel
type: Number
attributes:<optional>
default: 0
description: Zoom level to use when image is first opened or the home button is clicked. If 0, adjusts to fit viewer.
name: opacity
type: Number
attributes:<optional>
default: 1
description: Default opacity of the tiled images (1=opaque, 0=transparent)
name: compositeOperation
type: String
attributes:<optional>
default: null
description: Valid values are 'source-over', 'source-atop', 'source-in', 'source-out', 'destination-over', 'destination-atop', 'destination-in', 'destination-out', 'lighter', 'copy' or 'xor'
name: placeholderFillStyle
type: String | CanvasGradient | CanvasPattern | function
attributes:<optional>
default: null
description: Draws a colored rectangle behind the tile if it is not loaded yet. You can pass a CSS color value like "#FF8800". When passing a function the tiledImage and canvas context are available as argument which is useful when you draw a gradient or pattern.
name: degrees
type: Number
attributes:<optional>
default: 0
description: Initial rotation.
name: minZoomLevel
type: Number
attributes:<optional>
default: null
description:
name: maxZoomLevel
type: Number
attributes:<optional>
default: null
description:
name: homeFillsViewer
type: Boolean
attributes:<optional>
default: false
description: Make the 'home' button fill the viewer and clip the image, instead of fitting the image to the viewer and letterboxing.
name: panHorizontal
type: Boolean
attributes:<optional>
default: true
description: Allow horizontal pan.
name: panVertical
type: Boolean
attributes:<optional>
default: true
description: Allow vertical pan.
name: constrainDuringPan
type: Boolean
attributes:<optional>
default: false
description:
name: wrapHorizontal
type: Boolean
attributes:<optional>
default: false
description: Set to true to force the image to wrap horizontally within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.
name: wrapVertical
type: Boolean
attributes:<optional>
default: false
description: Set to true to force the image to wrap vertically within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.
name: minZoomImageRatio
type: Number
attributes:<optional>
default: 0.9
description: The minimum percentage ( expressed as a number between 0 and 1 ) of the viewport height or width at which the zoom out will be constrained. Setting it to 0, for example will allow you to zoom out infinity.
name: maxZoomPixelRatio
type: Number
attributes:<optional>
default: 1.1
description: The maximum ratio to allow a zoom-in to affect the highest level pixel ratio. This can be set to Infinity to allow 'infinite' zooming into the image though it is less effective visually if the HTML5 Canvas is not availble on the viewing device.
name: smoothTileEdgesMinZoom
type: Number
attributes:<optional>
default: 1.1
description: A zoom percentage ( where 1 is 100% ) of the highest resolution level. When zoomed in beyond this value alternative compositing will be used to smooth out the edges between tiles. This will have a performance impact. Can be set to Infinity to turn it off. Note: This setting is ignored on iOS devices due to a known bug (See https://github.com/openseadragon/openseadragon/issues/952)
name: iOSDevice
type: Boolean
attributes:<optional>
default: ?
description: True if running on an iOS device, false otherwise. Used to disable certain features that behave differently on iOS devices.
name: autoResize
type: Boolean
attributes:<optional>
default: true
description: Set to false to prevent polling for viewer size changes. Useful for providing custom resize behavior.
name: preserveImageSizeOnResize
type: Boolean
attributes:<optional>
default: false
description: Set to true to have the image size preserved when the viewer is resized. This requires autoResize=true (default).
name: minScrollDeltaTime
type: Number
attributes:<optional>
default: 50
description: Number of milliseconds between canvas-scroll events. This value helps normalize the rate of canvas-scroll events between different devices, causing the faster devices to slow down enough to make the zoom control more manageable.
name: pixelsPerWheelLine
type: Number
attributes:<optional>
default: 40
description: For pixel-resolution scrolling devices, the number of pixels equal to one scroll line.
name: visibilityRatio
type: Number
attributes:<optional>
default: 0.5
description: The percentage ( as a number from 0 to 1 ) of the source image which must be kept within the viewport. If the image is dragged beyond that limit, it will 'bounce' back until the minimum visibility ratio is achieved. Setting this to 0 and wrapHorizontal ( or wrapVertical ) to true will provide the effect of an infinitely scrolling viewport.
name: viewportMargins
type: Object
attributes:<optional>
default: {}
description: Pushes the "home" region in from the sides by the specified amounts. Possible subproperties (Numbers, in screen coordinates): left, top, right, bottom.
name: imageLoaderLimit
type: Number
attributes:<optional>
default: 0
description: The maximum number of image requests to make concurrently. By default it is set to 0 allowing the browser to make the maximum number of image requests in parallel as allowed by the browsers policy.
name: clickTimeThreshold
type: Number
attributes:<optional>
default: 300
description: The number of milliseconds within which a pointer down-up event combination will be treated as a click gesture.
name: clickDistThreshold
type: Number
attributes:<optional>
default: 5
description: The maximum distance allowed between a pointer down event and a pointer up event to be treated as a click gesture.
name: dblClickTimeThreshold
type: Number
attributes:<optional>
default: 300
description: The number of milliseconds within which two pointer down-up event combinations will be treated as a double-click gesture.
name: dblClickDistThreshold
type: Number
attributes:<optional>
default: 20
description: The maximum distance allowed between two pointer click events to be treated as a double-click gesture.
name: springStiffness
type: Number
attributes:<optional>
default: 6.5
description:
name: animationTime
type: Number
attributes:<optional>
default: 1.2
description: Specifies the animation duration per each OpenSeadragon.Spring which occur when the image is dragged or zoomed.
name: gestureSettingsMouse
type: OpenSeadragon.GestureSettings
attributes:<optional>
default: Settings for gestures generated by a mouse pointer device. (See OpenSeadragon.GestureSettings)
description: Properties

name: Name
type: Type
attributes:Attributes
default: Default
description: Description
name: scrollToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on scroll gesture
name: clickToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on click gesture
name: dblClickToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.
name: pinchToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on pinch gesture
name: flickEnabled
type: Boolean
attributes:<optional>
default: false
description: Enable flick gesture
name: flickMinSpeed
type: Number
attributes:<optional>
default: 120
description: If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second)
name: flickMomentum
type: Number
attributes:<optional>
default: 0.25
description: If flickEnabled is true, the momentum factor for the flick gesture
name: pinchRotate
type: Boolean
attributes:<optional>
default: false
description: If pinchRotate is true, the user will have the ability to rotate the image using their fingers.
name: gestureSettingsTouch
type: OpenSeadragon.GestureSettings
attributes:<optional>
default: Settings for gestures generated by a touch pointer device. (See OpenSeadragon.GestureSettings)
description: Properties
name: Name
type: Type
attributes:Attributes
default: Default
description: Description
name: scrollToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on scroll gesture
name: clickToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on click gesture
name: dblClickToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.
name: pinchToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on pinch gesture
name: flickEnabled
type: Boolean
attributes:<optional>
default: true
description: Enable flick gesture
name: flickMinSpeed
type: Number
attributes:<optional>
default: 120
description: If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second)
name: flickMomentum
type: Number
attributes:<optional>
default: 0.25
description: If flickEnabled is true, the momentum factor for the flick gesture
name: pinchRotate
type: Boolean
attributes:<optional>
default: false
description: If pinchRotate is true, the user will have the ability to rotate the image using their fingers.
name: gestureSettingsPen
type: OpenSeadragon.GestureSettings
attributes:<optional>
default: Settings for gestures generated by a pen pointer device. (See OpenSeadragon.GestureSettings)
description: Properties
name: Name
type: Type
attributes:Attributes
default: Default
description: Description
name: scrollToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on scroll gesture
name: clickToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on click gesture
name: dblClickToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.
name: pinchToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on pinch gesture
name: flickEnabled
type: Boolean
attributes:<optional>
default: false
description: Enable flick gesture
name: flickMinSpeed
type: Number
attributes:<optional>
default: 120
description: If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second)
name: flickMomentum
type: Number
attributes:<optional>
default: 0.25
description: If flickEnabled is true, the momentum factor for the flick gesture
name: pinchRotate
type: Boolean
attributes:<optional>
default: false
description: If pinchRotate is true, the user will have the ability to rotate the image using their fingers.
name: gestureSettingsUnknown
type: OpenSeadragon.GestureSettings
attributes:<optional>
default: Settings for gestures generated by unknown pointer devices. (See OpenSeadragon.GestureSettings)
description: Properties
name: Name
type: Type
attributes:Attributes
default: Default
description: Description
name: scrollToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on scroll gesture
name: clickToZoom
type: Boolean
attributes:<optional>
default: false
description: Zoom on click gesture
name: dblClickToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.
name: pinchToZoom
type: Boolean
attributes:<optional>
default: true
description: Zoom on pinch gesture
name: flickEnabled
type: Boolean
attributes:<optional>
default: true
description: Enable flick gesture
name: flickMinSpeed
type: Number
attributes:<optional>
default: 120
description: If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second)
name: flickMomentum
type: Number
attributes:<optional>
default: 0.25
description: If flickEnabled is true, the momentum factor for the flick gesture
name: pinchRotate
type: Boolean
attributes:<optional>
default: false
description: If pinchRotate is true, the user will have the ability to rotate the image using their fingers.
name: zoomPerClick
type: Number
attributes:<optional>
default: 2.0
description: The "zoom distance" per mouse click or touch tap. Note: Setting this to 1.0 effectively disables the click-to-zoom feature (also see gestureSettings[Mouse|Touch|Pen].clickToZoom/dblClickToZoom).
name: zoomPerScroll
type: Number
attributes:<optional>
default: 1.2
description: The "zoom distance" per mouse scroll or touch pinch. Note: Setting this to 1.0 effectively disables the mouse-wheel zoom feature (also see gestureSettings[Mouse|Touch|Pen].scrollToZoom}).
name: zoomPerSecond
type: Number
attributes:<optional>
default: 1.0
description: The number of seconds to animate a single zoom event over.
name: showNavigator
type: Boolean
attributes:<optional>
default: false
description: Set to true to make the navigator minimap appear.
name: navigatorId
type: String
attributes:<optional>
default: navigator-GENERATED DATE
description: The ID of a div to hold the navigator minimap. If an ID is specified, the navigatorPosition, navigatorSizeRatio, navigatorMaintainSizeRatio, navigator[Top|Left|Height|Width] and navigatorAutoFade options will be ignored. If an ID is not specified, a div element will be generated and placed on top of the main image.
name: navigatorPosition
type: String
attributes:<optional>
default: 'TOP_RIGHT'
description: Valid values are 'TOP_LEFT', 'TOP_RIGHT', 'BOTTOM_LEFT', 'BOTTOM_RIGHT', or 'ABSOLUTE'.
name: If 'ABSOLUTE' is specified, then navigator[Top|Left|Height|Width] determines the size and position of the navigator minimap in the viewer, and navigatorSizeRatio and navigatorMaintainSizeRatio are ignored.
type: For 'TOP_LEFT', 'TOP_RIGHT', 'BOTTOM_LEFT', and 'BOTTOM_RIGHT', the navigatorSizeRatio or navigator[Height|Width] values determine the size of the navigator minimap.
attributes:navigatorSizeRatio
default: Number
description: <optional>
name: 0.2
type: Ratio of navigator size to viewer size. Ignored if navigator[Height|Width] are specified.
attributes:navigatorMaintainSizeRatio
default: Boolean
description: <optional>
name: false
type: If true, the navigator minimap is resized (using navigatorSizeRatio) when the viewer size changes.
attributes:navigatorTop
default: Number | String
description: <optional>
name: null
type: Specifies the location of the navigator minimap (see navigatorPosition).
attributes:navigatorLeft
default: Number | String
description: <optional>
name: null
type: Specifies the location of the navigator minimap (see navigatorPosition).
attributes:navigatorHeight
default: Number | String
description: <optional>
name: null
type: Specifies the size of the navigator minimap (see navigatorPosition). If specified, navigatorSizeRatio and navigatorMaintainSizeRatio are ignored.
attributes:navigatorWidth
default: Number | String
description: <optional>
name: null
type: Specifies the size of the navigator minimap (see navigatorPosition). If specified, navigatorSizeRatio and navigatorMaintainSizeRatio are ignored.
attributes:navigatorAutoResize
default: Boolean
description: <optional>
name: true
type: Set to false to prevent polling for navigator size changes. Useful for providing custom resize behavior. Setting to false can also improve performance when the navigator is configured to a fixed size.
attributes:navigatorAutoFade
default: Boolean
description: <optional>
name: true
type: If the user stops interacting with the viewport, fade the navigator minimap. Setting to false will make the navigator minimap always visible.
attributes:navigatorRotate
default: Boolean
description: <optional>
name: true
type: If true, the navigator will be rotated together with the viewer.
attributes:controlsFadeDelay
default: Number
description: <optional>
name: 2000
type: The number of milliseconds to wait once the user has stopped interacting with the interface before begining to fade the controls. Assumes showNavigationControl and autoHideControls are both true.
attributes:controlsFadeLength
default: Number
description: <optional>
name: 1500
type: The number of milliseconds to animate the controls fading out.
attributes:maxImageCacheCount
default: Number
description: <optional>
name: 200
type: The max number of images we should keep in memory (per drawer).
attributes:timeout
default: Number
description: <optional>
name: 30000
type:
attributes:useCanvas
default: Boolean
description: <optional>
name: true
type: Set to false to not use an HTML canvas element for image rendering even if canvas is supported.
attributes:minPixelRatio
default: Number
description: <optional>
name: 0.5
type: The higher the minPixelRatio, the lower the quality of the image that is considered sufficient to stop rendering a given zoom level. For example, if you are targeting mobile devices with less bandwith you may try setting this to 1.5 or higher.
attributes:mouseNavEnabled
default: Boolean
description: <optional>
name: true
type: Is the user able to interact with the image via mouse or touch. Default interactions include draging the image in a plane, and zooming in toward and away from the image.
attributes:showNavigationControl
default: Boolean
description: <optional>
name: true
type: Set to false to prevent the appearance of the default navigation controls.
attributes:Note that if set to false, the customs buttons set by the options zoomInButton, zoomOutButton etc, are rendered inactive.
default: navigationControlAnchor
description: OpenSeadragon.ControlAnchor
name: <optional>
type: TOP_LEFT
attributes:Placement of the default navigation controls. To set the placement of the sequence controls, see the sequenceControlAnchor option.
default: showZoomControl
description: Boolean
name: <optional>
type: true
attributes:If true then + and - buttons to zoom in and out are displayed.
default: Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
description: showHomeControl
name: Boolean
type: <optional>
attributes:true
default: If true then the 'Go home' button is displayed to go back to the original zoom and pan.
description: Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
name: showFullPageControl
type: Boolean
attributes:<optional>
default: true
description: If true then the 'Toggle full page' button is displayed to switch between full page and normal mode.
name: Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
type: showRotationControl
attributes:Boolean
default: <optional>
description: false
name: If true then the rotate left/right controls will be displayed as part of the standard controls. This is also subject to the browser support for rotate (e.g. viewer.drawer.canRotate()).
type: Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.
attributes:showSequenceControl
default: Boolean
description: <optional>
name: true
type: If sequenceMode is true, then provide buttons for navigating forward and backward through the images.
attributes:sequenceControlAnchor
default: OpenSeadragon.ControlAnchor
description: <optional>
name: TOP_LEFT
type: Placement of the default sequence controls.
attributes:navPrevNextWrap
default: Boolean
description: <optional>
name: false
type: If true then the 'previous' button will wrap to the last image when viewing the first image and the 'next' button will wrap to the first image when viewing the last image.
attributes:zoomInButton
default: String
description:
name:
type: Set the id of the custom 'Zoom in' button to use. This is useful to have a custom button anywhere in the web page.
attributes:To only change the button images, consider using OpenSeadragon.Options.navImages
default: zoomOutButton
description: String
name:
type:
attributes:Set the id of the custom 'Zoom out' button to use. This is useful to have a custom button anywhere in the web page.
default: To only change the button images, consider using OpenSeadragon.Options.navImages
description: homeButton
name: String
type:
attributes:
default: Set the id of the custom 'Go home' button to use. This is useful to have a custom button anywhere in the web page.
description: To only change the button images, consider using OpenSeadragon.Options.navImages
name: fullPageButton
type: String
attributes:
default:
description: Set the id of the custom 'Toggle full page' button to use. This is useful to have a custom button anywhere in the web page.
name: To only change the button images, consider using OpenSeadragon.Options.navImages
type: rotateLeftButton
attributes:String
default:
description:
name: Set the id of the custom 'Rotate left' button to use. This is useful to have a custom button anywhere in the web page.
type: To only change the button images, consider using OpenSeadragon.Options.navImages
attributes:rotateRightButton
default: String
description:
name:
type: Set the id of the custom 'Rotate right' button to use. This is useful to have a custom button anywhere in the web page.
attributes:To only change the button images, consider using OpenSeadragon.Options.navImages
default: previousButton
description: String


Set the id of the custom 'Previous page' button to use. This is useful to have a custom button anywhere in the web page.
To only change the button images, consider using OpenSeadragon.Options.navImages
nextButton
String


Set the id of the custom 'Next page' button to use. This is useful to have a custom button anywhere in the web page.
To only change the button images, consider using OpenSeadragon.Options.navImages
sequenceMode
Boolean
<optional>
false
Set to true to have the viewer treat your tilesources as a sequence of images to be opened one at a time rather than all at once.
initialPage
Number
<optional>
0
If sequenceMode is true, display this page initially.
preserveViewport
Boolean
<optional>
false
If sequenceMode is true, then normally navigating through each image resets the viewport to 'home' position. If preserveViewport is set to true, then the viewport position is preserved when navigating between images in the sequence.
preserveOverlays
Boolean
<optional>
false
If sequenceMode is true, then normally navigating through each image resets the overlays. If preserveOverlays is set to true, then the overlays added with OpenSeadragon.Viewer#addOverlay are preserved when navigating between images in the sequence. Note: setting preserveOverlays overrides any overlays specified in the global "overlays" option for the Viewer. It's also not compatible with specifying per-tileSource overlays via the options, as those overlays will persist even after the tileSource is closed.
showReferenceStrip
Boolean
<optional>
false
If sequenceMode is true, then display a scrolling strip of image thumbnails for navigating through the images.
referenceStripScroll
String
<optional>
'horizontal'

referenceStripElement
Element
<optional>
null

referenceStripHeight
Number
<optional>
null

referenceStripWidth
Number
<optional>
null

referenceStripPosition
String
<optional>
'BOTTOM_LEFT'

referenceStripSizeRatio
Number
<optional>
0.2

collectionMode
Boolean
<optional>
false
Set to true to have the viewer arrange your TiledImages in a grid or line.
collectionRows
Number
<optional>
3
If collectionMode is true, specifies how many rows the grid should have. Use 1 to make a line. If collectionLayout is 'vertical', specifies how many columns instead.
collectionColumns
Number
<optional>
0
If collectionMode is true, specifies how many columns the grid should have. Use 1 to make a line. If collectionLayout is 'vertical', specifies how many rows instead. Ignored if collectionRows is not set to a falsy value.
collectionLayout
String
<optional>
'horizontal'
If collectionMode is true, specifies whether to arrange vertically or horizontally.
collectionTileSize
Number
<optional>
800
If collectionMode is true, specifies the size, in viewport coordinates, for each TiledImage to fit into. The TiledImage will be centered within a square of the specified size.
collectionTileMargin
Number
<optional>
80
If collectionMode is true, specifies the margin, in viewport coordinates, between each TiledImage.
crossOriginPolicy
String | Boolean
<optional>
false
Valid values are 'Anonymous', 'use-credentials', and false. If false, canvas requests will not use CORS, and the canvas will be tainted.
ajaxWithCredentials
Boolean
<optional>
false
Whether to set the withCredentials XHR flag for AJAX requests (when loading tile sources). Note that this can be overridden at the OpenSeadragon.TileSource level.
 */
