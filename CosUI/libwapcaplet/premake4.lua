solution'wapcaplet'
configurations{'debug','release'}
project'wapcaplet'
  kind'StaticLib'
  language'C'
  defines{'_BSD_SOURCE'}
  files{
     'include/**.h',
     'src/**.c',
  }
  includedirs{
     'include',
  }
