{
  'target_defaults': {
    'conditions': [
      ['OS=="win"', {
        'msvs_disabled_warnings': [

        ],
      }],
    ],
  },
  'targets': [
    {
      'target_name': 'keyboard',
      'sources': [
        'src/main.cc',
        'src/common.h',
        'src/common.cc',
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'src/keyboard_win.cc',
          ],
          'libraries': [
            '-lkernel32.lib',
            '-luser32.lib'
          ],
        }],
        ['OS=="mac"', {
          'sources': [
            'src/keyboard_mac.cc',
          ],
          'libraries': [

          ],
        }],
        ['OS=="linux"', {
          'sources': [
            'src/keyboard_linux.cc',
          ],
        }],
      ],
    }
  ]
}
