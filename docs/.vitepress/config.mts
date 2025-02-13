import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "filerix",
  description: "File manager library that provides all the essentials.",
  base: "/filerix/",
  cleanUrls: true,
  mpa: true,
  head: [
    [
      'link', 
      { rel: 'icon', href: 'favicon.ico' }
    ],
    [
      'meta',
      { name: 'google-site-verification', content: 'Y102h-h5HYlYeh1duBU7Tiw4hWdBh3kJcE42t1goHKI' }
    ]
  ],
  sitemap: {
    hostname: 'https://filesverse.github.io/filerix'
  },
  themeConfig: {
    logo: '/logo.png',

    nav: [
      { text: 'Home', link: '/' },
      { text: "Guide", link: '/guide/installation' },
    ],

    sidebar: [
      {
        text: 'Introduction',
        items: [
          { text: 'installation', link: '/guide/installation' },
          { text: 'Getting Started', link: '/guide/getting-started' },
          { text: 'Logging', link: '/guide/logging' }
        ]
      },
      {
        text: 'Usage',
        items: [
          {
            text: 'C++',
            items: [
              { text: 'cmake', link: '/guide/usage/cpp-cmake' },
              { text: 'meson', link: '/guide/usage/cpp-meson' },
            ]
          },
          {
            text: 'C',
            items: [
              { text: 'cmake', link: '/guide/usage/c-cmake' },
              { text: 'meson', link: '/guide/usage/c-meson' },
            ]
          },
          {
            text: 'nodejs',
            link: '/guide/usage/nodejs',
          },
          {
            text: 'electron',
            link: '/guide/usage/electron',
          },
        ]
      },
      {
        text: 'References',
        items: [
          { text: 'Compression', link: '/guide/references/compression' },
          { text: 'Drives', link: '/guide/references/drives' },
          { text: 'Filesystem', link: '/guide/references/filesystem' },
          { text: 'User', link: '/guide/references/user' },
        ]
      },
    ],
    socialLinks: [
      { icon: 'github', link: 'https://github.com/KingMaj0r/filerix' }
    ]
  }
})
