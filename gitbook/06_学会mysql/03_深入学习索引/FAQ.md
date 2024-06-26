# FAQ

## mysql 主键索引和唯一索引有什么区别？

MySQL 中的主键索引和唯一索引虽然都能确保数据记录的唯一性，但它们还是存在一些重要区别:

- 作用范围不同
  - 主键索引:每个表最多只能有一个主键，可以由一个或多个字段组成。主键索引可以确保表中记录的唯一性。
  - 唯一索引:每个表可以有多个唯一索引，每个唯一索引由一个或多个字段组成。唯一索引可以确保指定字段值在当前索引中的唯一性。

- 空值处理不同
  - 主键索引:主键索引列值不允许为 NULL。
  - 唯一索引:唯一索引列值允许有一个 NULL 值，但只能有一个。

- 自动创建方式不同
  - 主键索引:InnoDB 存储引擎会自动为主键创建聚簇索引。
  - 唯一索引:InnoDB 只会为唯一索引创建一个二级索引。

- 查询效率不同
  - 主键索引查询效率很高，MySQL 会直接通过主键索引查找数据位置。
  - 唯一索引需要先通过二级索引查到主键值，再通过主键索引查找数据位置，效率较低。
- 占用存储空间不同
  - 主键索引占用存储空间较少。
  - 唯一索引会占用较多存储空间，因为需要单独存储索引数据。
- 修改灵活性不同
  - 修改主键索引较为复杂，很多情况下需要重建表。
  - 修改唯一索引相对简单，可以直接对索引进行更新。

总的来说，主键索引是更为专一、高效的索引类型，可以有效确保数据记录的唯一性。而唯一索引提供了更多灵活性，可以满足一些特殊的需求。在设计表结构时，应该根据实际需求合理选择使用主键索引或唯一索引。

## mysql 中每个索引都是单独的文件存储的吗？

不是的，MySQL 中的索引并不是每个索引都单独存储为一个文件。

在 MySQL 中，索引的存储方式取决于存储引擎的实现。以最常用的 InnoDB 存储引擎为例，它是这样存储索引的:

- 主键索引(聚簇索引)：主键索引的数据是与表数据存储在同一个文件中的，也就是说主键索引是存储在表的 .ibd 文件内的。InnoDB 使用了聚簇索引文件存储行数据，这种数据存储方式提高了 I/O 性能。

- 二级索引(辅助索引/非聚簇索引)：除了主键索引以外的其他索引都属于二级索引，它们是独立存储的。 InnoDB 会为每个二级索引在共享表空间数据文件中单独开辟一个索引页面，存储的数据包括索引字段值及对应的主键值。

所以从这个层面来看，主键索引和表数据存储在一起，二级索引是单独存储的。但它们并不是单独的文件，而是存储在同一个表空间数据文件中。

另外，MySQL 8.0 之后还支持通过参数将二级索引也单独存储到 .ibd 文件中:

```sql
innodb_indexes_persisted=1
```

这种存储方式可以提高写性能，但会占用更多磁盘空间。

总的来说，MySQL 的索引存储方式是将数据行和主键索引存储在一起，二级索引独立存储但存放位置由参数控制。索引并不是每个都单独存储为一个文件，而是共享表空间数据文件。这种设计的目的是提高查询效率和 I/O 性能。

## 覆盖索引是怎么一回事？

覆盖索引(Covering Index)是一种查询优化技术，指的是在执行查询操作时，所需的数据可以直接从索引文件中获取，而无需查询数据文件。

当查询语句仅使用到构成索引的字段时，如果存储引擎已经为这些字段构造了一个索引，那么存储引擎就可以直接从索引文件中读取所需的数据，而不需要查询数据文件，这种索引就称为覆盖索引。

使用覆盖索引可以有效地减少存储引擎需要执行的工作，从而提高查询效率，主要有以下优势:

1. **减少回表操作**
   正常情况下，在执行查询时需要先从索引文件中获取数据的主键值(或其他唯一索引值)，然后再根据主键到数据文件中查询完整的数据记录，这个过程称为"回表"。使用覆盖索引就不需要回表操作了。

2. **减少I/O操作**
   不需要查询数据文件，只需要查询较小的索引文件，可以减少 I/O 操作，提高查询效率。

3. **提高缓存命中率**
   索引文件通常比数据文件更小，更容易被系统缓存命中。

4. **避免劫持主键索引**
   如果不使用覆盖索引，当只需要查询非主键字段时，还是需要通过主键索引来查询数据文件，可能会阻碍其他需要使用主键索引的查询。

需要注意的是，覆盖索引只能满足查询中所需的数据在索引文件中是可以获取到的，如果查询中包含的字段超出了索引字段范围，就无法使用覆盖索引。

另外，对于频繁更新的数据，使用覆盖索引也可能会增加维护索引的成本。因此，在设计索引时需要权衡查询效率和维护成本，才能发挥索引的优势。