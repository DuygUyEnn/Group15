using Microsoft.EntityFrameworkCore;
using WeatherApi.Models;

namespace WeatherApi.Data
{
    public class WeatherDbContext : DbContext
    {
        public WeatherDbContext(DbContextOptions<WeatherDbContext> options) : base(options) { }

        public DbSet<WeatherData> messages { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            // Đảm bảo rằng bảng 'messages' sẽ được sử dụng thay vì tạo bảng tự động
            modelBuilder.Entity<WeatherData>()
                .ToTable("messages");  // Chỉ định rõ tên bảng là 'messages'

            modelBuilder.Entity<WeatherData>()
                .HasKey(w => new { w.Time, w.DeviceId }); // Cặp khóa chính (composite key)
        }
    }
}
