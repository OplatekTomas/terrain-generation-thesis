## Libs
- nlohmann json
- cpr
- boost
- cpm

## Useful odkazy
- https://app.MapGenerator.io/

## Detaily
- uživatel si ozančí ma mapě oblast - vygeneruje se 3d mapa, kde se na základě metadat dogenerujou další věci
- gegl
- qt
- výškové data - bing
- metadata - openstreetmap

## Do ledna
 - načtení kusu mapy a drobné modifikace
 - úvod, teorie část návrhu do ledna, ale když to nebude tak hlavu mi neurve, ale tak 10 normostran
 - konzultovat hodně


## Notes
 - max res per request: 32x32 (1024 je max počet bodů)
 - transform feedback
 - tessalation shader
 - dummy vao


## Problémy
 - Random rozlišení (hack že podoporované rozlišení musí být scale x32)
 - gpuengine (fixed)

 
## OSM query

- základní query na získání všech dat v oblasti
```
[out:json];
(
  node({{bbox}});
  <;
);
out meta;
```

- Vyfiltrovat way prakticky takhle `data.Elements.Where(x => x.Type == TypeEnum.Way && x.Nodes is not null).Where(x=>x.Tags is not null)`
- To mi vrátí všechny prvky s informacema o oblastech, domech atd atd
- Pak v nodes je seznam nodů které ten objekt tvoří
- Ty si můžu mrdnout třeba do dictionary a pak k nim přistupovat s O(1)
