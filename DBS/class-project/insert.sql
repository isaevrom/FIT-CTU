exec VYMAZ_DATA_VSECH_TABULEK;
-- ZACATEK: zapneme proceduru pro utlum erroru kvuli FK
-- alter table => ddl prikaz => implicitne provede commit
exec VYPNI_CIZI_KLICE;

-- SLUZBA_TYP data: 	  SLUZBA_TYP_KEY, NAZEV
  insert into SLUZBA_TYP (SLUZBA_TYP_KEY, NAZEV) values (SEQ_SLUZBA_TYP_KEY.nextval, 'Mesic bazen');
  insert into SLUZBA_TYP (SLUZBA_TYP_KEY, NAZEV) values (SEQ_SLUZBA_TYP_KEY.nextval, 'Mesic fitko');
  insert into SLUZBA_TYP (SLUZBA_TYP_KEY, NAZEV) values (SEQ_SLUZBA_TYP_KEY.nextval, 'Mesic sauna');
  insert into SLUZBA_TYP (SLUZBA_TYP_KEY, NAZEV) values (SEQ_SLUZBA_TYP_KEY.nextval, 'Mesic multivstup');
  commit;
-- END;

-- VYBAVENI data:         VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Fitko', 'zavazi, bezecke pasy', 2 );
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Bazen', 'plavecky bazen', 1 );
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Sauna', 'vytapena mistnost', 3 );
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'F+B+S', 'Fitko, Bazen, Sauna', 4 );
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Fotbalovy mic', 'Fotbal se tu nevede', null);
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Mec', 'Sermiri zde nejsou', null);
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Kohoutek na vodu', 'rezava..', 1);
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Kohoutek na vodu', 'rezava..', 2);
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Kohoutek na vodu', 'rezava..', 3);
  insert into VYBAVENI (VYBAVENI_KEY, NAZEV, POPIS, SLUZBA_TYP_KEY) values (SEQ_VYBAVENI_KEY.nextval, 'Kohoutek na vodu', 'rezava..', 4);

-- END;


-- ADRESA data:        ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Praha', 'Prazska', '86', '19000', 1);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Hindi', 'Urdu', '53', '1337', 2);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Kosice', 'Koziskova', '253', '1148', 1);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Vocel', 'Zelezarna', '34', '15698', 1);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Brazzaville', 'Mokaci', '17', '145', 7);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Moskva', 'Moskevska', '15', '44000', 9);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Changsha', '976 Carey Circle', '8079', null, 5);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Zhangfeng', '7 Briar Crest Junction', '9085', null, 6);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Mengla', '81 Sutherland Place', '041', null, 7);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Fatukanutu', '8 Merrick Avenue', '711', null, 8);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Nioro', '2161 Starling Court', '9', null, 9);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Wanjia', '046 Marcy Way', '312', null, 10);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Zhishan', '34 Sundown Plaza', '8850', null, 11);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Vejlet', 'Prazska', '86', '19000', 1);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Praha', 'Letna', '76', '13000', 1);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Praha', 'Prazska', '14', '17000', 1);
  insert into ADRESA (ADRESA_KEY, MESTO, ULICE, CISLO, PSC, STAT_KEY) values (SEQ_ADRESA_KEY.nextval, 'Praha', 'BOob', '47', null, 1);
-- END;


-- POBOCKA data:         POBOCKA_KEY, NAZEV, ADRESA_KEY
  insert into POBOCKA (POBOCKA_KEY, NAZEV, ADRESA_KEY) values (SEQ_POBOCKA_KEY.nextval, 'OC Harfa', 1);
  insert into POBOCKA (POBOCKA_KEY, NAZEV, ADRESA_KEY) values (SEQ_POBOCKA_KEY.nextval, 'Hobbiti nora', 4);
  insert into POBOCKA (POBOCKA_KEY, NAZEV, ADRESA_KEY) values (SEQ_POBOCKA_KEY.nextval, 'Kebab Star', 5);
-- END;



-- SLUZBA data:       SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '11.11.2018', '11.12.2018', 1, 4);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '03.10.2015', '03.11.2015', 2, 2);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '07.03.2020', '07.04.2020', 1, 1);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '02.08.2019', '02.09.2019', 2, 4);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '02.09.2019', '02.10.2019', 2, 4);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '03.10.2019', '03.11.2019', 2, 4);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '16.06.2019', '16.07.2019', 2, 3);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '24.10.2020', '24.11.2020', 2, 4);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '02.08.2019', '02.09.2019', 1, 4);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '02.08.2019', '02.09.2019', 2, 2);
  insert into SLUZBA (SLUZBA_KEY, DATUM_ZACATKU_VYUZITI, DATUM_KONCE_VYUZITI, POBOCKA_KEY, SLUZBA_TYP_KEY) values (SEQ_SLUZBA_KEY.nextval, '05.05.2017', '05.06.2017', 2, 2);
-- END;


-- STAT data:   	STAT_KEY, NAZEV
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Ceska republika');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Indie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Spojene staty americke');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Indonesie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Brazilie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Pakistan');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Nigerie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Velka Britanie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Rusko');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Japonsko');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Spanelsko');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Kolumbie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Ukrajina');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Tanzanie');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Argentina');
  insert into STAT (STAT_KEY, NAZEV) values (SEQ_STAT_KEY.nextval, 'Kena');
  commit;
-- END;

-- ZAKAZNIK data:       ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Alfonse Lohde', '9307753969', 'alohde0@angelfire.com', 'Acrobates pygmaeus', 17);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Vivien Deerness', '3352750948', 'vdeerness1@flickr.com', 'Podargus strigoides', 2);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Sheffie Meins', '8864913729', 'smeins2@qq.com', 'Numida meleagris', 3);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Roxane Lyver', '9033506441', 'rlyver3@toplist.cz', 'Gorilla gorilla', 4);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Levey Grut', '6183207920', 'lgrut4@jiathis.com', 'Nannopterum harrisi', 5);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Wallas McGaffey', '7671827776', 'wmcgaffey5@icio.us', 'Agkistrodon piscivorus', 6);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Lucy Al Hirsi', '9607662639', 'lal6@jugem.jp', 'Cercatetus concinnus', 7);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Janeva Childes', '7575893515', 'jchildes7@jigsy.com', 'Larus novaehollandiae', 8);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Karoly Robak', '5383712689', 'krobak8@freewebs.com', 'Bubalornis niger', 9);
  insert into ZAKAZNIK (ZAKAZNIK_KEY, JMENO, TELEFON, EMAIL, POZNAMKA, ADRESA_KEY) values (SEQ_ZAKAZNIK_KEY.nextval, 'Dominic D''Souza', '1439131673', 'ddsouza9@reddit.com', 'Eudyptula minor', 10);
-- END

-- OBJEDNAVKA data:    OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 5000, 'Miluju vas', 4, 7);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 80000, 'Zbytek si nechte', 5, 11);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 5000, 'Mesic sportu!', 7, 4);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 7000, 'Platba 7000', 1, 3);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 66664, 'Platba 66664', 3, 8);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 65556, 'Platba 65556', 8, 7);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 66666, 'Platba 66666', 4, 1);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 6666, 'Platba 6666', 3, 4);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 4000, 'Srdecne posilam 4000!', 2, 1);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 5000, 'Platba 4000', 5, 1);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 4000, 'Srdecne posilam 4000', 2, 2);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 4000, 'Srdecne posilam 4000', 2, 3);
  insert into OBJEDNAVKA (OBJEDNAVKA_KEY, CASTKA, POPIS, ZAKAZNIK_KEY, SLUZBA_KEY) values (SEQ_OBJEDNAVKA_KEY.nextval, 4000, 'Srdecne posilam 4000', 2, 7);
-- END

-- ZAMESTESTNANEC_POZICE data:
  insert into ZAMESTNANEC_POZICE (POZICE_KEY, NAZEV) values (SEQ_POZICE_KEY.nextval, 'Hlavni manazer' );
  insert into ZAMESTNANEC_POZICE (POZICE_KEY, NAZEV) values (SEQ_POZICE_KEY.nextval, 'Podrizeny Workoholik' );
  insert into ZAMESTNANEC_POZICE (POZICE_KEY, NAZEV) values (SEQ_POZICE_KEY.nextval, 'Podrizeny Flakac' );
-- END

-- ZAMESTNANEC data:    ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Constancy Mattessen', to_date('30.09.1974', 'DD.MM.YY'), '7168058075', 'cmattessen0@issuu.com', 1, 1);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Mayor Pinkney', '15.12.1981', '4204637604', 'mpinkney1@myspace.com', 2, 2);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Hewie Deniso', '29.05.1985', '8663875608', 'hdeniso2@drupal.org', 3, 2);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Tessie Lydden', '16.11.1990', '5072790869', 'tlydden3@un.org', 2, 1);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Lorelle Hacquard', '03.06.1988', '6409593574', 'lhacquard4@salon.com', 2, 2);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Zondra Sweedland', '08.01.1963', '1306489913', 'zsweedland5@illinois.edu', 2, 1);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Aldis Ascraft', '05.02.1997', '1312240150', 'aascraft6@nih.gov', 2, 1);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Ivar Heskins', '16.10.1997', '7252047560', 'iheskins7@1und1.de', 2, 2);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Sam Guierre', '05.05.1957', '4088662338', 'sguierre8@cdc.gov', 3, 1);
  insert into ZAMESTNANEC (ZAMESTNANEC_KEY, JMENO, DATUM_NAROZENI, TELEFON, EMAIL, POZICE_KEY, POBOCKA_KEY) values (SEQ_ZAMESTNANEC_KEY.nextval, 'Russ Rennie', '11.01.1998', '6723452122', 'rrennie9@list-manage.com', 3, 2);
-- END
commit;

-- UKLIZECKA data:                  	UKLIDOVE_NARADI, ZAMESTNANEC_KEY
  insert into UKLIZECKA (UKLIDOVE_NARADI, ZAMESTNANEC_KEY) values ('Benzocaine', 2);
  insert into UKLIZECKA (UKLIDOVE_NARADI, ZAMESTNANEC_KEY) values ('ACETAMINOPHEN-PHENYLEPHRINE', 3);
  insert into UKLIZECKA (UKLIDOVE_NARADI, ZAMESTNANEC_KEY) values ('OCTINOXATE, TITANIUM DIOXIDE', 4);
  insert into UKLIZECKA (UKLIDOVE_NARADI, ZAMESTNANEC_KEY) values ('Fosinopril Sodium', 5);
-- END

-- PRACOVNIK_ADMINISTRACE data:         STAV_UNIFORMY, ZAMESTNANEC_KEY
  insert into PRACOVNIK_ADMINISTRACE (STAV_UNIFORMY, ZAMESTNANEC_KEY) values ('Up-sized', 1);
  insert into PRACOVNIK_ADMINISTRACE (STAV_UNIFORMY, ZAMESTNANEC_KEY) values ('product', 6);
  insert into PRACOVNIK_ADMINISTRACE (STAV_UNIFORMY, ZAMESTNANEC_KEY) values ('didactic', 7);
  insert into PRACOVNIK_ADMINISTRACE (STAV_UNIFORMY, ZAMESTNANEC_KEY) values ('systematic', 8);
  insert into PRACOVNIK_ADMINISTRACE (STAV_UNIFORMY, ZAMESTNANEC_KEY) values ('high-level', 9);
  insert into PRACOVNIK_ADMINISTRACE (STAV_UNIFORMY, ZAMESTNANEC_KEY) values ('Future-proofed', 10);
-- END

-- KONEC: vratime vse do puvodniho stavu
-- alter table => ddl prikaz => implicitne provede commit
exec ZAPNI_CIZI_KLICE;
