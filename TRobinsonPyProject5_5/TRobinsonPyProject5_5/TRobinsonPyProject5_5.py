import sqlite3 as sl

con = sl.connect('mymovies.db')
c = con.cursor()
c.execute(''' SELECT count(name) FROM sqlite_master WHERE type='table' AND name='MOVIE' ''')

if c.fetchone()[0]==1:
    print('Table exists.')
else:
    print('Table does not exist.')

    with con:
        con.execute("""CREATE TABLE MOVIE (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, name TEXT, releaseDate INTEGER);""")
        sql='INSERT INTO MOVIE(id,name,releaseDate)values(?,?,?)'
        data=[(1,'The Adventures of Mark Twain',1985), 
             (2,'Labyrinth',1986),
             (3,'The Dark Crystal',1982), 
             (4,'The Lord of the Rings: the Fellowship of the Ring', 2001),
             (5,'The Matrix',1999),
             (6,'The Terminator',1984),
             (7,'Little Shop of Horrors',1986),
             (8,'Harry Potter and the Sorcerer\'s Stone',2001),
             (9,'The NeverEnding Story',1984),
             (10,'The Hobbit: An Unexpected Journey',2012)]
        with con:
            con.executemany(sql, data)
            with con:
                data=con.execute("SELECT * FROM MOVIE WHERE releaseDate >= 2000")
                print("All films made during or after the year 2000.")
                for row in data:
                    print(row)
                print()
                print("All films made before the year 2000.")
                data=con.execute("SELECT * FROM MOVIE WHERE releaseDate <= 2000")
                for row in data:
                    print(row)