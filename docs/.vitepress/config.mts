import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "filerix",
  description: "File manager library that provides all the essentials.",
  base: "/filerix/",
  cleanUrls: true,
  head: [
    [
      'link', 
      { rel: 'icon', href: 'favicon.ico' }
    ],
    [
      'script',
      { async: '', src: 'https://www.googletagmanager.com/gtag/js?id=googlecbcab056e50286fb' }
    ],
    [
      'script',
      {},
      `window.dataLayer = window.dataLayer || [];
      function gtag(){dataLayer.push(arguments);}
      gtag('js', new Date());
      gtag('config', 'googlecbcab056e50286fb');`
    ]
  ],
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
