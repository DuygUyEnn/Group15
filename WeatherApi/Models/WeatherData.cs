using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;
using System.Numerics;

namespace WeatherApi.Models
{
    [Table("messages")]
    public class WeatherData
    {
        [Column("time")][Key] public DateTime Time { get; set; }

        [Column("device_id")][Key] public string DeviceId { get; set; }
        [Column("timestamp")] public long timestamp { get; set; }

        [Column("humidity")] public float humidity { get; set; }

        [Column("light")] public float light { get; set; }

        [Column("pressure")] public float pressure { get; set; }

        [Column("temp")] public float temperature { get; set; }
        // Additional properties can be added here
    }
}
