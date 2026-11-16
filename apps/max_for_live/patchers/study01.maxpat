{
  "patcher": {
    "fileversion": 1,
    "appversion": {
      "major": 8,
      "minor": 6,
      "revision": 0,
      "architecture": "x64",
      "modernui": 1
    },
    "classnamespace": "box",
    "rect": [
      0.0,
      0.0,
      820.0,
      500.0
    ],
    "openinpresentation": 0,
    "default_fontsize": 12.0,
    "default_fontface": 0,
    "default_fontname": "Arial",
    "boxes": [
      {
        "box": {
          "id": "title",
          "maxclass": "comment",
          "patching_rect": [
            30.0,
            15.0,
            740.0,
            30.0
          ],
          "text": "algorhythmia / SYS 01 \u2014 Max for Live integration patch",
          "fontsize": 20.0
        }
      },
      {
        "box": {
          "id": "note",
          "maxclass": "comment",
          "patching_rect": [
            30.0,
            50.0,
            760.0,
            40.0
          ],
          "text": "enable DRIVE to hear bytebeat or cellular fm. live transform needs stereo input. this editable integration patch is not a packaged .amxd.",
          "linecount": 2
        }
      },
      {
        "box": {
          "id": "input",
          "maxclass": "newobj",
          "patching_rect": [
            35.0,
            300.0,
            70.0,
            22.0
          ],
          "text": "plugin~",
          "numinlets": 0,
          "numoutlets": 2,
          "outlettype": [
            "signal",
            "signal"
          ]
        }
      },
      {
        "box": {
          "id": "engine",
          "maxclass": "newobj",
          "patching_rect": [
            285.0,
            355.0,
            120.0,
            22.0
          ],
          "text": "algorhythmia~",
          "numinlets": 2,
          "numoutlets": 2,
          "outlettype": [
            "signal",
            "signal"
          ]
        }
      },
      {
        "box": {
          "id": "output",
          "maxclass": "newobj",
          "patching_rect": [
            290.0,
            425.0,
            75.0,
            22.0
          ],
          "text": "plugout~",
          "numinlets": 2,
          "numoutlets": 0
        }
      },
      {
        "box": {
          "id": "time-label",
          "maxclass": "comment",
          "patching_rect": [
            35.0,
            110.0,
            150.0,
            20.0
          ],
          "text": "FLUX"
        }
      },
      {
        "box": {
          "id": "time",
          "maxclass": "flonum",
          "patching_rect": [
            35.0,
            140.0,
            70.0,
            24.0
          ],
          "numinlets": 1,
          "numoutlets": 2,
          "minimum": 0.0,
          "maximum": 1.0
        }
      },
      {
        "box": {
          "id": "time-message",
          "maxclass": "newobj",
          "patching_rect": [
            35.0,
            185.0,
            110.0,
            22.0
          ],
          "text": "prepend flux",
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "form-label",
          "maxclass": "comment",
          "patching_rect": [
            215.0,
            110.0,
            150.0,
            20.0
          ],
          "text": "BIAS"
        }
      },
      {
        "box": {
          "id": "form",
          "maxclass": "flonum",
          "patching_rect": [
            215.0,
            140.0,
            70.0,
            24.0
          ],
          "numinlets": 1,
          "numoutlets": 2,
          "minimum": 0.0,
          "maximum": 1.0
        }
      },
      {
        "box": {
          "id": "form-message",
          "maxclass": "newobj",
          "patching_rect": [
            215.0,
            185.0,
            110.0,
            22.0
          ],
          "text": "prepend bias",
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "play-label",
          "maxclass": "comment",
          "patching_rect": [
            395.0,
            110.0,
            150.0,
            20.0
          ],
          "text": "DRIVE"
        }
      },
      {
        "box": {
          "id": "play",
          "maxclass": "toggle",
          "patching_rect": [
            395.0,
            140.0,
            24.0,
            24.0
          ],
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "play-message",
          "maxclass": "newobj",
          "patching_rect": [
            395.0,
            185.0,
            110.0,
            22.0
          ],
          "text": "prepend drive",
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "mode-label",
          "maxclass": "comment",
          "patching_rect": [
            575.0,
            110.0,
            150.0,
            20.0
          ],
          "text": "STATE (bank)"
        }
      },
      {
        "box": {
          "id": "mode",
          "maxclass": "toggle",
          "patching_rect": [
            575.0,
            140.0,
            24.0,
            24.0
          ],
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "mode-message",
          "maxclass": "newobj",
          "patching_rect": [
            575.0,
            185.0,
            110.0,
            22.0
          ],
          "text": "prepend state",
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "time-default",
          "maxclass": "newobj",
          "patching_rect": [
            35.0,
            230.0,
            90.0,
            22.0
          ],
          "text": "loadmess 0.5",
          "numinlets": 1,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "scenes-label",
          "maxclass": "comment",
          "patching_rect": [
            400.0,
            245.0,
            370.0,
            22.0
          ],
          "text": "Scene 0: Bytebeat / 1: Cellular FM / 2: Live Transform"
        }
      },
      {
        "box": {
          "id": "scene0",
          "maxclass": "message",
          "patching_rect": [
            420.0,
            280.0,
            80.0,
            22.0
          ],
          "text": "scene 0",
          "numinlets": 2,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "scene1",
          "maxclass": "message",
          "patching_rect": [
            520.0,
            280.0,
            80.0,
            22.0
          ],
          "text": "scene 1",
          "numinlets": 2,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "scene2",
          "maxclass": "message",
          "patching_rect": [
            620.0,
            280.0,
            80.0,
            22.0
          ],
          "text": "scene 2",
          "numinlets": 2,
          "numoutlets": 1
        }
      },
      {
        "box": {
          "id": "scene-note",
          "maxclass": "comment",
          "patching_rect": [
            410.0,
            315.0,
            330.0,
            45.0
          ],
          "text": "STATE selects the bytebeat bank here. Mapping a hardware STATE tap to scene cycling belongs to the future controller adapter.",
          "linecount": 3
        }
      },
      {
        "box": {
          "id": "form-default",
          "maxclass": "newobj",
          "patching_rect": [
            215.0,
            230.0,
            90.0,
            22.0
          ],
          "text": "loadmess 0.5",
          "numinlets": 1,
          "numoutlets": 1
        }
      }
    ],
    "lines": [
      {
        "patchline": {
          "source": [
            "input",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "engine",
            0
          ],
          "destination": [
            "output",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "input",
            1
          ],
          "destination": [
            "engine",
            1
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "engine",
            1
          ],
          "destination": [
            "output",
            1
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "time",
            0
          ],
          "destination": [
            "time-message",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "time-message",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "form",
            0
          ],
          "destination": [
            "form-message",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "form-message",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "play",
            0
          ],
          "destination": [
            "play-message",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "play-message",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "mode",
            0
          ],
          "destination": [
            "mode-message",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "mode-message",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "time-default",
            0
          ],
          "destination": [
            "time",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "scene0",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "scene1",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "scene2",
            0
          ],
          "destination": [
            "engine",
            0
          ]
        }
      },
      {
        "patchline": {
          "source": [
            "form-default",
            0
          ],
          "destination": [
            "form",
            0
          ]
        }
      }
    ]
  }
}
