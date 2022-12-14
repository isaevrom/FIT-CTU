-- Generated by Oracle SQL Developer Data Modeler 4.1.3.901
--   at:        2017-05-23 16:42:17 CEST
--   site:      Oracle Database 11g
--   type:      Oracle Database 11g

prompt #---------------------#
prompt #- Pomocne procedury -#
prompt #---------------------#

create or replace procedure SMAZ_VSECHNY_TABULKY AS
-- pokud v logu bude uvedeno, ze nektery objekt nebyl zrusen, protoze na nej jiny jeste existujici objekt stavi, spust proceduru opakovane, dokud se nezrusi vse
begin
  for iRec in
    (select distinct OBJECT_TYPE, OBJECT_NAME,
      'drop '||OBJECT_TYPE||' "'||OBJECT_NAME||'"'||
      case OBJECT_TYPE when 'TABLE' then ' cascade constraints purge' else ' ' end as PRIKAZ
    from USER_OBJECTS where OBJECT_NAME not in ('SMAZ_VSECHNY_TABULKY', 'VYPNI_CIZI_KLICE', 'ZAPNI_CIZI_KLICE', 'VYMAZ_DATA_VSECH_TABULEK')
    ) loop
        begin
          dbms_output.put_line('Prikaz: '||irec.prikaz);
        execute immediate iRec.prikaz;
        exception
          when others then dbms_output.put_line('NEPOVEDLO SE!');
        end;
      end loop;
end;
/

create or replace procedure VYPNI_CIZI_KLICE as
begin
  for cur in (select CONSTRAINT_NAME, TABLE_NAME from USER_CONSTRAINTS where CONSTRAINT_TYPE = 'R' )
  loop
    execute immediate 'alter table '||cur.TABLE_NAME||' modify constraint "'||cur.CONSTRAINT_NAME||'" DISABLE';
  end loop;
end VYPNI_CIZI_KLICE;
/


create or replace procedure ZAPNI_CIZI_KLICE as
begin
  for cur in (select CONSTRAINT_NAME, TABLE_NAME from USER_CONSTRAINTS where CONSTRAINT_TYPE = 'R' )
  loop
    execute immediate 'alter table '||cur.TABLE_NAME||' modify constraint "'||cur.CONSTRAINT_NAME||'" enable validate';
  end loop;
end ZAPNI_CIZI_KLICE;
/

create or replace procedure VYMAZ_DATA_VSECH_TABULEK is
begin
  -- Vymazat data vsech tabulek
  VYPNI_CIZI_KLICE;
  for v_rec in (select distinct TABLE_NAME from USER_TABLES)
  loop
    execute immediate 'truncate table '||v_rec.TABLE_NAME||' drop storage';
  end loop;
  ZAPNI_CIZI_KLICE;

  -- Nastavit vsechny sekvence od 1
  for v_rec in (select distinct SEQUENCE_NAME  from USER_SEQUENCES)
  loop
    execute immediate 'alter sequence '||v_rec.SEQUENCE_NAME||' restart start with 1';
  end loop;
end VYMAZ_DATA_VSECH_TABULEK;
/

prompt #------------------------#
prompt #- Zrusit stare tabulky -#
prompt #------------------------#

exec SMAZ_VSECHNY_TABULKY;

prompt #-------------------------#
prompt #- Vytvorit nove tabulky -#
prompt #-------------------------#


alter session set nls_date_format = 'DD.MM.YY';

CREATE TABLE adresa (
    adresa_key  INTEGER NOT NULL,
    mesto       VARCHAR2(25 CHAR) NOT NULL,
    ulice       VARCHAR2(35 CHAR),
    cislo       VARCHAR2(8 CHAR) NOT NULL,
    psc         VARCHAR2(8 CHAR),
    stat_key    INTEGER NOT NULL
);

ALTER TABLE adresa ADD CONSTRAINT pk_adresa PRIMARY KEY ( adresa_key );

CREATE TABLE objednavka (
    objednavka_key  INTEGER NOT NULL,
    castka          INTEGER NOT NULL,
    popis           VARCHAR2(45 CHAR) NOT NULL,
    zakaznik_key    INTEGER NOT NULL,
    sluzba_key      INTEGER NOT NULL
);

ALTER TABLE objednavka ADD CONSTRAINT pk_objednavka PRIMARY KEY ( objednavka_key );

CREATE TABLE pobocka (
    pobocka_key  INTEGER NOT NULL,
    nazev        VARCHAR2(40 CHAR) NOT NULL,
    adresa_key   INTEGER NOT NULL
);

ALTER TABLE pobocka ADD CONSTRAINT pk_pobocka PRIMARY KEY ( pobocka_key );

CREATE TABLE pracovnik_administrace (
    stav_uniformy    VARCHAR2(15 CHAR) NOT NULL,
    zamestnanec_key  INTEGER NOT NULL
);

ALTER TABLE pracovnik_administrace ADD CONSTRAINT pk_administrace PRIMARY KEY ( zamestnanec_key );

CREATE TABLE sluzba (
    sluzba_key             INTEGER NOT NULL,
    datum_zacatku_vyuziti  DATE NOT NULL,
    datum_konce_vyuziti    DATE NOT NULL,
    pobocka_key            INTEGER NOT NULL,
    sluzba_typ_key         INTEGER NOT NULL
);

ALTER TABLE sluzba ADD CONSTRAINT pk_sluzba PRIMARY KEY ( sluzba_key );

CREATE TABLE sluzba_typ (
    sluzba_typ_key  INTEGER NOT NULL,
    nazev           VARCHAR2(20 CHAR) NOT NULL
);

ALTER TABLE sluzba_typ ADD CONSTRAINT pk_sluzba_typ PRIMARY KEY ( sluzba_typ_key );

CREATE TABLE stat (
    stat_key  INTEGER NOT NULL,
    nazev     VARCHAR2(35 CHAR) NOT NULL
);

ALTER TABLE stat ADD CONSTRAINT pk_stat PRIMARY KEY ( stat_key );

CREATE TABLE uklizecka (
    uklidove_naradi  VARCHAR2(30 CHAR) NOT NULL,
    zamestnanec_key  INTEGER NOT NULL
);

ALTER TABLE uklizecka ADD CONSTRAINT pk_uklizecka PRIMARY KEY ( zamestnanec_key );

CREATE TABLE vybaveni (
    vybaveni_key    INTEGER NOT NULL,
    nazev           VARCHAR2(20 CHAR) NOT NULL,
    popis           VARCHAR2(30 CHAR),
    sluzba_typ_key  INTEGER
);

ALTER TABLE vybaveni ADD CONSTRAINT pk_vybaveni PRIMARY KEY ( vybaveni_key );

CREATE TABLE zakaznik (
    zakaznik_key  INTEGER NOT NULL,
    jmeno         VARCHAR2(30 CHAR) NOT NULL,
    telefon       VARCHAR2(20 CHAR),
    email         VARCHAR2(40 CHAR),
    poznamka      VARCHAR2(30 CHAR),
    adresa_key    INTEGER NOT NULL
);

ALTER TABLE zakaznik ADD CONSTRAINT pk_zakaznik PRIMARY KEY ( zakaznik_key );

CREATE TABLE zamestnanec (
    zamestnanec_key  INTEGER NOT NULL,
    jmeno            VARCHAR2(40) NOT NULL,
    datum_narozeni   DATE NOT NULL,
    telefon          VARCHAR2(20 CHAR),
    email            VARCHAR2(40 CHAR),
    pozice_key       INTEGER NOT NULL,
    pobocka_key      INTEGER NOT NULL
);

ALTER TABLE zamestnanec ADD CONSTRAINT pk_zamestnanec PRIMARY KEY ( zamestnanec_key );

CREATE TABLE zamestnanec_pozice (
    pozice_key  INTEGER NOT NULL,
    nazev       VARCHAR2(25 CHAR) NOT NULL
);

ALTER TABLE zamestnanec_pozice ADD CONSTRAINT pk_pozice PRIMARY KEY ( pozice_key );

ALTER TABLE pracovnik_administrace
    ADD CONSTRAINT fk_administrace_zamestnanec FOREIGN KEY ( zamestnanec_key )
        REFERENCES zamestnanec ( zamestnanec_key );

ALTER TABLE adresa
    ADD CONSTRAINT fk_adresa_stat FOREIGN KEY ( stat_key )
        REFERENCES stat ( stat_key );

ALTER TABLE objednavka
    ADD CONSTRAINT fk_objednavka_sluzba FOREIGN KEY ( sluzba_key )
        REFERENCES sluzba ( sluzba_key );

ALTER TABLE objednavka
    ADD CONSTRAINT fk_objednavka_zakaznik FOREIGN KEY ( zakaznik_key )
        REFERENCES zakaznik ( zakaznik_key );

ALTER TABLE pobocka
    ADD CONSTRAINT fk_pobocka_adresa FOREIGN KEY ( adresa_key )
        REFERENCES adresa ( adresa_key );

ALTER TABLE sluzba
    ADD CONSTRAINT fk_sluzba_pobocka FOREIGN KEY ( pobocka_key )
        REFERENCES pobocka ( pobocka_key );

ALTER TABLE sluzba
    ADD CONSTRAINT fk_sluzba_sluzba_typ FOREIGN KEY ( sluzba_typ_key )
        REFERENCES sluzba_typ ( sluzba_typ_key );

ALTER TABLE uklizecka
    ADD CONSTRAINT fk_uklizecka_zamestnanec FOREIGN KEY ( zamestnanec_key )
        REFERENCES zamestnanec ( zamestnanec_key );

ALTER TABLE vybaveni
    ADD CONSTRAINT fk_vybaveni_sluzba_typ FOREIGN KEY ( sluzba_typ_key )
        REFERENCES sluzba_typ ( sluzba_typ_key );

ALTER TABLE zakaznik
    ADD CONSTRAINT fk_zakaznik_adresa FOREIGN KEY ( adresa_key )
        REFERENCES adresa ( adresa_key );

ALTER TABLE zamestnanec
    ADD CONSTRAINT fk_zamestnanec_pobocka FOREIGN KEY ( pobocka_key )
        REFERENCES pobocka ( pobocka_key );

ALTER TABLE zamestnanec
    ADD CONSTRAINT fk_zamestnanec_pozice FOREIGN KEY ( pozice_key )
        REFERENCES zamestnanec_pozice ( pozice_key );


CREATE SEQUENCE SEQ_ADRESA_KEY      START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_OBJEDNAVKA_KEY  START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_POBOCKA_KEY     START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_SLUZBA_KEY      START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_SLUZBA_TYP_KEY  START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_STAT_KEY        START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_VYBAVENI_KEY    START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_ZAKAZNIK_KEY    START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_ZAMESTNANEC_KEY START WITH 1 NOCACHE ORDER ;
CREATE SEQUENCE SEQ_POZICE_KEY      START WITH 1 NOCACHE ORDER ;

-- Oracle SQL Developer Data Modeler Summary Report:
--
-- CREATE TABLE                            12
-- CREATE INDEX                             0
-- ALTER TABLE                             24
-- CREATE VIEW                              0
-- ALTER VIEW                               0
-- CREATE PACKAGE                           0
-- CREATE PACKAGE BODY                      0
-- CREATE PROCEDURE                         0
-- CREATE FUNCTION                          0
-- CREATE TRIGGER                           0
-- ALTER TRIGGER                            0
-- CREATE COLLECTION TYPE                   0
-- CREATE STRUCTURED TYPE                   0
-- CREATE STRUCTURED TYPE BODY              0
-- CREATE CLUSTER                           0
-- CREATE CONTEXT                           0
-- CREATE DATABASE                          0
-- CREATE DIMENSION                         0
-- CREATE DIRECTORY                         0
-- CREATE DISK GROUP                        0
-- CREATE ROLE                              0
-- CREATE ROLLBACK SEGMENT                  0
-- CREATE SEQUENCE                          0
-- CREATE MATERIALIZED VIEW                 0
-- CREATE MATERIALIZED VIEW LOG             0
-- CREATE SYNONYM                           0
-- CREATE TABLESPACE                        0
-- CREATE USER                              0
--
-- DROP TABLESPACE                          0
-- DROP DATABASE                            0
--
-- REDACTION POLICY                         0
--
-- ORDS DROP SCHEMA                         0
-- ORDS ENABLE SCHEMA                       0
-- ORDS ENABLE OBJECT                       0
--
-- ERRORS                                   0
-- WARNINGS                                 0
